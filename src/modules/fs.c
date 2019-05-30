/*
module:
  name: FS
  author: Shiroko
  summary: FS Process
  module_task: Task_FS
*/

#include "kernel/memory.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "modules/hd.h"
#include "modules/modules.h"
#include "syscall/syscall.h"

struct FAT32_BS {
  u8 jmpBoot[3];
  uchar OEMName[8];
  u16 BytesPerSec;
  u8 SecPerClus;
  u16 RsvdSecCnt;
  u8 NumFATs;
  u16 RootEntCnt; // 0 in fat 32
  u16 TotSec16;
  u8 Media;
  u16 FATSz16; // 0 in fat 32
  u16 SecPerTrk;
  u16 NumHeads;
  u32 HiddSec;
  u32 TotSec32;
  //=======
  u32 FATSz32;
  u16 ExtFlags;
  u16 FSVer;
  u32 RootClus;
  u16 FSInfo;
  u16 BkBootSec;
  u8 Reserved[12];
  u8 DrvNum;
  u8 Reserved1;
  u8 BootSig; // 0x29
  u32 VolID;
  char VolLab[11];
  u8 FileSysType[8];
} __attribute__((packed));

struct FAT32_DirEnt {
  uchar Name[8];
  uchar Ext[3];
  u8 Attr;
  u8 NTRes;
  u8 CrtTimeTenth;
  u16 CrtTime;
  u16 CrtDate;
  u16 LastAccDate;
  u16 FstClusHI;
  u16 WrtTime;
  u16 WrtDate;
  u16 FstClusLO;
  u32 FileSize;
} __attribute__((packed));

struct FAT32_LongDirEnt {
  u8 Ord;
  char Name1[0]; // 2B a char, totally 5 chars;
  u8 Attr;
  u8 Type;
  u8 ChkSum;
  char Name2[12]; // 2B a char, totally 6 chars;
  u16 FstClusLO;  // be zero
  char Name3[4];  // 2B a char, totally 2 chars;
} __attribute__((packed));

struct FAT32_FSInfo {
  u32 StrucSig;
  u32 FreeCount;
  u32 NxtFree;
  u8 Rsvd[12];
  uint TrailSig;
} __attribute__((packed));

struct FAT32_FileSystem {
  uchar OEMName[9];
  size_t BytesPerSec;
  uint SecPerClus;
  ubyte NumFATs;
  uint HiddenSector;
  uint TotalSector;
  uint FATSize; // in sector
  u16 ExtFlags;
  uint RootClus;
  u16 FSInfo;
  u16 BackupBootSector;
  u8 Signature;
  uint VolumeID;
  char VolumeLabel[12];
  char FileSystemType[9];
  ushort drv;
  uint FreeClusCount;
  uint NextFreeClusCount;
};

void ReadBootSector(ushort drv, struct FAT32_FileSystem *fs) {
  u8 hd_buf[512];
  HD_drv_read(drv, 0, hd_buf, 512);
  struct FAT32_BS BootSector;
  memset(&BootSector, 0, sizeof(struct FAT32_BS));
  memcpy(&BootSector, hd_buf, sizeof(struct FAT32_BS));
  char VolLab[12];
  char OEMName[9];
  char FileSysType[9];
  memset(VolLab, 0, sizeof(VolLab));
  memset(OEMName, 0, sizeof(OEMName));
  memset(FileSysType, 0, sizeof(FileSysType));
  memcpy(VolLab, BootSector.VolLab, 11);
  memcpy(OEMName, BootSector.OEMName, 8);
  memcpy(FileSysType, BootSector.FileSysType, 8);
#if __DEBUG__ && __FS_DEBUG__
  printf("[FS] jmpBoot: 0x%x 0x%x 0x%x, OEMName: %s, BytesPerSec: %d, "
         "SecPerClus: %d, "
         "RsvdSecCnt: %d, NumFATs: %d, RootEntCnt: %d, TotSec16: %d, Media: "
         "%d, FATSz16: %d, SecPerTrk: %d, NumHeads: %d, HiddSec: %d\n",
         BootSector.jmpBoot[0], BootSector.jmpBoot[1], BootSector.jmpBoot[2],
         OEMName, BootSector.BytesPerSec, BootSector.SecPerClus,
         BootSector.RsvdSecCnt, BootSector.NumFATs, BootSector.RootEntCnt,
         BootSector.TotSec16, BootSector.Media, BootSector.FATSz16,
         BootSector.SecPerTrk, BootSector.NumHeads, BootSector.HiddSec);
  printf(
      "[FS] TotSec32: %d, FATSz32: %d, ExtFlags: 0x%x, FSVer: 0x%x, RootClus: "
      "%d, FSInfo: %d, BkBootSec: %d, DrvNum: %d, BootSig: 0x%x, VolID: %d, "
      "VolLab: %s\n",
      BootSector.TotSec32, BootSector.FATSz32, BootSector.ExtFlags,
      BootSector.FSVer, BootSector.RootClus, BootSector.FSInfo,
      BootSector.BkBootSec, BootSector.DrvNum, BootSector.BootSig,
      BootSector.VolID, VolLab);
#endif
  assert(BootSector.BootSig == 0x29);
  assert(BootSector.TotSec16 == 0);
  assert(BootSector.FATSz16 == 0);
  assert(BootSector.RootEntCnt == 0);
  memset(fs, 0, sizeof(struct FAT32_FileSystem));
  memcpy(fs->OEMName, OEMName, sizeof(OEMName));
  memcpy(fs->VolumeLabel, VolLab, sizeof(VolLab));
  memcpy(fs->FileSystemType, FileSysType, sizeof(FileSysType));
  fs->BytesPerSec = BootSector.BytesPerSec;
  fs->SecPerClus = BootSector.SecPerClus;
  fs->NumFATs = BootSector.NumFATs;
  fs->HiddenSector = BootSector.HiddSec;
  fs->TotalSector = BootSector.TotSec32;
  fs->FATSize = BootSector.FATSz32;
  fs->ExtFlags = BootSector.ExtFlags;
  fs->RootClus = BootSector.RootClus;
  fs->FSInfo = BootSector.FSInfo;
  fs->BackupBootSector = BootSector.BkBootSec;
  fs->Signature = BootSector.BootSig;
  fs->VolumeID = BootSector.VolID;
  fs->drv = drv;
}

void ReadFSInfo(struct FAT32_FileSystem *fs) {
  ubyte hd_buf[512];
  HD_drv_read(fs->drv, fs->FSInfo, hd_buf, 512);
  struct FAT32_FSInfo FSInfo;
  uint FSInfo_LeadSig;
  memcpy(&FSInfo_LeadSig, hd_buf, 4);
  assert(FSInfo_LeadSig == 0x41615252);
  memcpy(&FSInfo, hd_buf + 484, sizeof(struct FAT32_FSInfo));
  assert(FSInfo.StrucSig == 0x61417272);
  assert(FSInfo.TrailSig == 0xAA550000);
  fs->FreeClusCount = FSInfo.FreeCount;
  fs->NextFreeClusCount = FSInfo.NxtFree;
}

void Task_FS() {
  delay_ms(200);
  printf("[FS] Initializing.\n");
  delay_ms(500);
  printf("Reading Boot Sector\n");
  HD_dev_open(0);
  struct FAT32_FileSystem fs;
  ReadBootSector(MAKE_DRV(0, 1), &fs);
  printf("Opened FS \"%s\", About %d MB.\n", fs.VolumeLabel,
         fs.TotalSector * fs.BytesPerSec / 1024 / 1024);
  ReadFSInfo(&fs);
  HD_dev_close(0);
  while (1) {
  }
}