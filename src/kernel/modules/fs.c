/*
module:
  name: FS
  author: Shiroko
  summary: FS Process
  entry: Task_FS
*/

#include "core/memory.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"
#include "modules/hd.h"
#include "modules/tty.h"

ubyte hd_buf[512];

struct FAT32_BS {
    u8    jmpBoot[3];
    uchar OEMName[8];
    u16   BytesPerSec;
    u8    SecPerClus;
    u16   RsvdSecCnt;
    u8    NumFATs;
    u16   RootEntCnt; // 0 in fat 32
    u16   TotSec16;
    u8    Media;
    u16   FATSz16; // 0 in fat 32
    u16   SecPerTrk;
    u16   NumHeads;
    u32   HiddSec;
    u32   TotSec32;
    //=======
    u32  FATSz32;
    u16  ExtFlags;
    u16  FSVer;
    u32  RootClus;
    u16  FSInfo;
    u16  BkBootSec;
    u8   Reserved[12];
    u8   DrvNum;
    u8   Reserved1;
    u8   BootSig; // 0x29
    u32  VolID;
    char VolLab[11];
    u8   FileSysType[8];
} __attribute__((packed));

union FAT32_DirEnt {
    struct {
        uchar Name[8];
        uchar Ext[3];
        u8    Attr;
        u8    NTRes;
        u8    CrtTimeTenth;
        u16   CrtTime;
        u16   CrtDate;
        u16   LastAccDate;
        u16   FstClusHI;
        u16   WrtTime;
        u16   WrtDate;
        u16   FstClusLO;
        u32   FileSize;
    } __attribute__((packed));

    struct {
        u8   L_Ord;
        char L_Name1[10]; // 2B a char, totally 5 chars;
        u8   L_Attr;
        u8   L_Type;
        u8   L_ChkSum;
        char L_Name2[12]; // 2B a char, totally 6 chars;
        u16  L_FstClusLO; // be zero
        char L_Name3[4];  // 2B a char, totally 2 chars;
    } __attribute__((packed));
};

struct FAT32_FSInfo {
    u32  StrucSig;
    u32  FreeCount;
    u32  NxtFree;
    u8   Rsvd[12];
    uint TrailSig;
} __attribute__((packed));

struct FAT32_FileSystem {
    uchar  OEMName[9];
    size_t BytesPerSec;
    uint   SecPerClus;
    ubyte  NumFATs;
    uint   HiddenSector;
    uint   TotalSector;
    uint   FATSize; // in sector
    u16    ExtFlags;
    uint   RootClus;
    u16    FSInfo;
    u16    BackupBootSector;
    u8     Signature;
    uint   VolumeID;
    char   VolumeLabel[12];
    char   FileSystemType[9];
    uint   FirstDataClus;
    ushort drv;
    uint   FreeClusCount;
    uint   NextFreeClusCount;
};

#define ATTR_READ_ONLY 0x1
#define ATTR_HIDDEN    0x2
#define ATTR_SYSTEM    0x4
#define ATTR_VOLUME_ID 0x8
#define ATTR_DIR       0x10
#define ATTR_ARCHIVE   0x20
#define FS_ATTR_LONGNAME                                                       \
    (ATTR_READ_ONLY | ATTR_VOLUME_ID | ATTR_SYSTEM | ATTR_HIDDEN)

#define CLUS2SECTOR(fs, N)                                                     \
    (((fs)->FirstDataClus) + ((N - (fs)->RootClus) * (fs)->SecPerClus))

// TrimWhiteSpace assume that len(dst) >= len(src)
void TrimWhiteSpace(const char *src, char *dst) {
    char *end;
    char *str = (char *)src;
    while ((uchar)(*(str)) == ' ')
        str++;
    if (*str == 0)
        return;
    end = str + strlen(str) - 1;
    while (end > str && ((uchar)(*(end)) == ' '))
        end--;
    memcpy(dst, str, end - str + 1);
}

void ReadBootSector(ushort drv, struct FAT32_FileSystem *fs) {
    struct HD_PartInfo part_info;
    HD_info(drv, &part_info);
    if (part_info.fsid != 0xC)
        panic("FS is not FAT32 LBA");
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
        "[FS] TotSec32: %d, FATSz32: %d, ExtFlags: 0x%x, FSVer: 0x%x, "
        "RootClus: "
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
    fs->BytesPerSec      = BootSector.BytesPerSec;
    fs->SecPerClus       = BootSector.SecPerClus;
    fs->NumFATs          = BootSector.NumFATs;
    fs->HiddenSector     = BootSector.HiddSec;
    fs->TotalSector      = BootSector.TotSec32;
    fs->FATSize          = BootSector.FATSz32;
    fs->ExtFlags         = BootSector.ExtFlags;
    fs->RootClus         = BootSector.RootClus;
    fs->FSInfo           = BootSector.FSInfo;
    fs->BackupBootSector = BootSector.BkBootSec;
    fs->Signature        = BootSector.BootSig;
    fs->VolumeID         = BootSector.VolID;
    fs->drv              = drv;
    fs->FirstDataClus =
        BootSector.RsvdSecCnt + BootSector.FATSz32 * BootSector.NumFATs;
}

static inline u32 get_next_clus_in_FAT(struct FAT32_FileSystem *fs, u32 clus) {
    u32 next_clus = 0;
    return next_clus;
}

void ReadFSInfo(struct FAT32_FileSystem *fs) {
    HD_drv_read(fs->drv, fs->FSInfo, hd_buf, 512);
    struct FAT32_FSInfo FSInfo;
    uint                FSInfo_LeadSig;
    memcpy(&FSInfo_LeadSig, hd_buf, 4);
    assert(FSInfo_LeadSig == 0x41615252);
    memcpy(&FSInfo, hd_buf + 484, sizeof(struct FAT32_FSInfo));
    assert(FSInfo.StrucSig == 0x61417272);
    assert(FSInfo.TrailSig == 0xAA550000);
    fs->FreeClusCount     = FSInfo.FreeCount;
    fs->NextFreeClusCount = FSInfo.NxtFree;
}

static u8 checksum_fname(char *fname) {
    u8 i;
    u8 checksum = 0;
    for (i = 0; i < 11; i++) {
        u8 highbit = (checksum & 0x1) << 7;
        checksum   = ((checksum >> 1) & 0x7F) | highbit;
        checksum   = checksum + fname[i];
    }
    return checksum;
}

static inline char char_upper(char c) {
    return c - (c >= 'a' && c <= 'z' ? 32 : 0);
}

// read_8_3_filename assume that thr length of buffer is >= 12
static void read_8_3_filename(uchar *fname, uchar *buffer) {
    char ext[3];
    char name[8];
    memcpy(ext, fname + 8, 3);
    memcpy(name, fname, 8);
    memset(buffer, 0, 12);
    TrimWhiteSpace(name, (char *)buffer);
    if (ext[0] == ' ')
        return;
    memset(buffer + strlen((const char *)buffer), '.', 1);
    memcpy(buffer + strlen((const char *)buffer), ext, 3);
}

static void write_8_3_filename(uchar *fname, uchar *buffer) {
    memset(buffer, ' ', 11);
    u32 namelen = strlen((const char *)fname);
    // find the extension
    int i;
    int dot_index = -1;
    for (i = namelen - 1; i >= 0; i--) {
        if (fname[i] == '.') {
            // Found it!
            dot_index = i;
            break;
        }
    }

    // Write the extension
    if (dot_index >= 0) {
        for (i = 0; i < 3; i++) {
            u32   c_index = dot_index + 1 + i;
            uchar c = c_index >= namelen ? ' ' : char_upper(fname[c_index]);
            buffer[8 + i] = c;
        }
    } else {
        for (i = 0; i < 3; i++) {
            buffer[8 + i] = ' ';
        }
    }

    // Write the filename.
    u32 firstpart_len = namelen;
    if (dot_index >= 0) {
        firstpart_len = dot_index;
    }
    if (firstpart_len > 8) {
        // Write the weird tilde thing.
        for (i = 0; i < 6; i++) {
            buffer[i] = char_upper(fname[i]);
        }
        buffer[6] = '~';
        buffer[7] = '1'; // probably need to enumerate like files and increment.
    } else {
        // Just write the file name.
        u32 j;
        for (j = 0; j < firstpart_len; j++) {
            buffer[j] = char_upper(fname[j]);
        }
    }
}

void list_dir(struct FAT32_FileSystem *fs, uint clus, uint tabsize) {
    uchar hd_buf[512];
    HD_drv_read(fs->drv, CLUS2SECTOR(fs, clus), hd_buf, 512);
    union FAT32_DirEnt DirEnt;
    for (uint offset = 0; offset < 512; offset += sizeof(union FAT32_DirEnt)) {
        memcpy(&DirEnt, hd_buf + offset, sizeof(union FAT32_DirEnt));
        if (DirEnt.Name[0] == 0)
            break;
        if (DirEnt.Name[0] == 0xE5 || DirEnt.Name[0] == 0x05)
            continue;
        if (DirEnt.Attr == FS_ATTR_LONGNAME)
            continue;
        if (DirEnt.Name[0] == 0x2E)
            continue;
        char buffer[16];
        memset(buffer, 0, 16);
        memset(buffer, ' ', tabsize * 4);
        printf("%s| ", buffer);
        uchar buf[12];
        read_8_3_filename(DirEnt.Name, buf);
        memset(buffer, 0, 16);
        memcpy(buffer, DirEnt.Name, 11);
        printf("%s\n", buf);
        if (DirEnt.Attr & ATTR_DIR) {
            /* list_dir(fs, DirEnt.FstClusHI << 16 | DirEnt.FstClusLO, */
            /* tabsize + 1); */
        }
    }
    // check whether this is the end of dir.
}

void Task_FS() {
    delay_ms(200);
    printf("[FS] Initializing.\n");
    delay_ms(500);
    printf("Reading Boot Sector\n");
    HD_dev_open(0);
    struct FAT32_FileSystem fs;
    ReadBootSector(MAKE_DRV(0, 1), &fs);
    char buf[8];
    memset(buf, 0, sizeof(buf));
    TrimWhiteSpace(fs.VolumeLabel, buf);
    printf("Opened FS \"%s\", About %d MB.\n", buf,
           fs.TotalSector * fs.BytesPerSec / 1024 / 1024);
    ReadFSInfo(&fs);
    /*
    HD_drv_read(fs.drv, CLUS2SECTOR(&fs, 2), hd_buf, 512);
    for (uint i = 0; i < 512; i++) {
      printf("%x ", hd_buf[i]);
    }
    */
    list_dir(&fs, fs.RootClus, 0); // 2 is root dir first clus, no more explain
    HD_dev_close(0);
    while (1) {
    }
}
