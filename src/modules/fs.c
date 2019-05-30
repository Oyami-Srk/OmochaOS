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

// Disk layout
// Superblock | inodes | inode bitmap | bitmap | data | journal
#define ROOT_INODE 1 // inode 0 is spare
#define BLOCK_SIZE 512

struct super_block {
  uchar magic[8]; // OSFSOSFS
  uint size;
  uint ninodes;       // inodes
  uint nblocks;       // data blocks
  uint njournal;      // journal
  uint offset_inodes; // in sector
  uint offset_bitmap;
  uint offset_inodebm;
  uint offset_data;
  uint offset_journal;
  // size 44
};

struct super_block_mem {
  uchar magic[8]; // OSFSOSFS
  uint size;
  uint ninodes;       // inodes
  uint nblocks;       // data blocks
  uint njournal;      // journal
  uint offset_inodes; // in sector
  uint offset_bitmap;
  uint offset_inodebm;
  uint offset_data;
  uint offset_journal;
  // size 44
  uchar *pInode_Bitmap;
  uchar *pBitmap;
};

#define BDIRECT 10
#define BINDIRECT 2
#define BTRIDIRECT 1
#define NINDIRECT ((BLOCK_SIZE / sizeof(uint)) * BINDIRECT)
#define NTRIDIRECT                                                             \
  ((BLOCK_SIZE / sizeof(uint)) * (BLOCK_SIZE / sizeof(uint)) * BTRIDIRECT)
#define MAX_BLOCKS (BDIRECT + NINDIRECT + NTRIDIRECT)
#define MAX_FILE_SIZE (BLOCK_SIZE * MAX_BLOCKS)

typedef struct {
  short type;
  short major;
  short minor;
  short link; // free when link is zero, creating file means link set to 1
  // 8 bytes above
  // 4+N bytes below
  uint size; // file size in bytes
  uint blocks[BDIRECT + BINDIRECT + BTRIDIRECT];
} inode;

#define DIRSIZE 14

typedef struct {
  unsigned short inum; // 2 bytes
  char name[DIRSIZE];  // 14 bytes
} dirent;

#define IPB (BLOCK_SIZE / sizeof(inode))
#define IBLOCK(i, sb) ((i) / IPB + sb.offset_inodes)
#define BPB (BLOCK_SIZE * 8)
// Block number of bitmap containing for b
#define BBLOCK(b, sb) (b / BPB + sb.offset_bitmap)

#define INODE_COUNT 1024
// (1024 * 64)
#define INODE_BLOCKS (INODE_COUNT / IPB + (INODE_COUNT % IPB == 0 ? 0 : 1))

#define INODE_BITMAP_SIZE (INODE_BLOCKS * IPB / 8)

#define TEST_FILE_SIZE (20 * 1024 * 1024)
#define TEST_FILE_SIZE_CHAR "20M"
#define _80MINBYTES (80 * 1024 * 1024)

void mkfs(uint part) {
  uint dev = 0;
  ubyte hd_buf[512];
  HD_dev_open(dev);
  struct HD_PartInfo part_info;
  uint drv = MAKE_DRV(dev, part);
  HD_info(drv, &part_info);
  printf("[FS] Making fs on dirk %d part %d, base(lba) is %d, size is %d MB\n",
         dev, part, part_info.base, part_info.size / 1024 / 1024);

  uint size_superblock = BLOCK_SIZE;
  uint size_inodes = INODE_COUNT * sizeof(inode);
  uint size_inodebm = INODE_BITMAP_SIZE;
  uint num_blocks = (part_info.size / BLOCK_SIZE);
  uint size_bitmap = num_blocks / 8 + (num_blocks % 8 == 0 ? 0 : 1);
  uint size_journal = 0; // currently not have journal

  uint offset_inodes = BLOCK_SIZE;
  uint offset_inodebm =
      BLOCK_SIZE * (offset_inodes / BLOCK_SIZE + INODE_BLOCKS);
  uint offset_bitmap =
      BLOCK_SIZE * (offset_inodebm / BLOCK_SIZE + size_inodebm / BLOCK_SIZE +
                    (size_inodebm % BLOCK_SIZE == 0 ? 0 : 1));
  uint offset_data =
      BLOCK_SIZE * (offset_bitmap / BLOCK_SIZE + size_bitmap / BLOCK_SIZE +
                    (size_bitmap % BLOCK_SIZE == 0 ? 0 : 1));

  printf("[FS] Size: inodes %d, inodebm %d, bitmap %d, data %d\n", size_inodes,
         size_inodebm, size_bitmap, 0);
  printf("[FS] Offset: inodes %d, inodebm: %d, bitmap: %d, data %d\n",
         offset_inodes, offset_inodebm, offset_bitmap, offset_data);

  assert(offset_inodebm % BLOCK_SIZE == 0);
  assert(offset_inodebm >= offset_inodes + size_inodes);
  assert(offset_bitmap % BLOCK_SIZE == 0);
  assert(offset_bitmap >= offset_inodebm + size_inodebm);
  assert(offset_data % BLOCK_SIZE == 0);
  assert(offset_data >= offset_bitmap + size_bitmap);

  struct super_block sb;
  memcpy((void *)sb.magic, "OMOCHAFS", 8);
  sb.size = part_info.size;
  sb.ninodes = INODE_COUNT;
  sb.nblocks = num_blocks;
  sb.njournal = 0;
  sb.offset_inodes = offset_inodes;
  sb.offset_inodebm = offset_inodebm;
  sb.offset_bitmap = offset_bitmap;
  sb.offset_data = offset_data;
  sb.offset_journal = sb.size;

  memset(hd_buf, 0, 512);
  memcpy(hd_buf, &sb, sizeof(struct super_block));
  HD_drv_write(drv, 0, hd_buf, 512);
  printf("[FS] Superblock wrote!\n");

  inode empty_inode_a_block[IPB];
  memset(empty_inode_a_block, 0, sizeof(inode) * IPB);
  for (uint addr = offset_inodes; addr < offset_inodebm; addr += BLOCK_SIZE)
    HD_drv_write(drv, addr / BLOCK_SIZE, (ubyte *)empty_inode_a_block,
                 BLOCK_SIZE);
  printf("[FS] Write all %d blocks for empty inodes.\n",
         (offset_inodebm - offset_inodes) / BLOCK_SIZE);

  uchar empty_bitmap_a_block[BLOCK_SIZE];
  memset(empty_bitmap_a_block, 0, sizeof(uchar) * BLOCK_SIZE);
  for (uint addr = offset_inodebm; addr < offset_bitmap; addr += BLOCK_SIZE)
    HD_drv_write(dev, addr / BLOCK_SIZE, (ubyte *)empty_bitmap_a_block,
                 BLOCK_SIZE);
  printf("[FS] Write all %d blocks for empty inode blocks.\n",
         (offset_bitmap - offset_inodebm) / BLOCK_SIZE);

  for (uint addr = offset_bitmap; addr < offset_data; addr += BLOCK_SIZE)
    HD_drv_write(drv, addr / BLOCK_SIZE, (ubyte *)empty_bitmap_a_block,
                 BLOCK_SIZE);
  printf("[FS] Write all %d blocks for empty block bitmap.\n",
         (offset_data - offset_bitmap) / BLOCK_SIZE);

  const char *test_data = "test data";
  HD_drv_write(drv, offset_data / BLOCK_SIZE, (ubyte *)test_data, 10);
  printf("[FS] Test data is Okey at 0x%x\n", offset_data);

  HD_dev_close(dev);
}

void Task_FS() {
  delay_ms(200);
  printf("[FS] Initializing.\n");
  mkfs(1);
  while (1) {
  }
}