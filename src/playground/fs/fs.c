// 用来做实验的，没什么意义的文件

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

// tools

#define INFO(indent, fmt, ...) printf("%*s" fmt "\n", (int)indent * 4 , "", ##__VA_ARGS__)

#define TEST_FILE_NAME "test.bin"

// definations

typedef unsigned int uint;
typedef unsigned char uchar;

// file system defination

// Disk layout:
// superblock | inodes | inode bitmap | bitmap | data | journal

#define ROOT_INODE 1
// block size is currently the size of a sector
#define BLOCK_SIZE 512

typedef struct {
  uchar magic[8];   // OMOCHAFS
  uint size;        //
  uint ninodes;     // inodes
  uint nblocks;     // data blocks
  uint njournal;    // journal
  uint offset_inodes;
  uint offset_bitmap;
  uint offset_inodebm;
  uint offset_data;
  uint offset_journal;
  //size 44
}superblock;

#define CACHE_SIZE 8

typedef struct {
  uchar magic[8];   // OMOCHAFS
  uint size;        //
  uint ninodes;     // inodes
  uint nblocks;     // data blocks
  uint njournal;    // journal
  uint offset_inodes;
  uint offset_bitmap;
  uint offset_inodebm;
  uint offset_data;
  uint offset_journal;
  // size 44
  // only represent in memory:
  uchar *pInode_Bitmap;
  uchar *pBitmap;
}sb_in_mem;

#define BDIRECT 10
#define BINDIRECT 2
#define BTRIDIRECT 1
#define NINDIRECT ((BLOCK_SIZE / sizeof(uint)) * BINDIRECT)
#define NTRIDIRECT ((BLOCK_SIZE / sizeof(uint)) * (BLOCK_SIZE / sizeof(uint)) * BTRIDIRECT)
#define MAX_BLOCKS (BDIRECT + NINDIRECT + NTRIDIRECT)
#define MAX_FILE_SIZE (BLOCK_SIZE * MAX_BLOCKS)

typedef struct {
  short type;
  short major;
  short minor;
  short link; // free when link is zero, creating file means link set to 1
  // 8 bytes above
  // 4+N bytes below
  uint size;  // file size in bytes
  uint blocks[BDIRECT + BINDIRECT + BTRIDIRECT];
}inode;

#define DIRSIZE 14

typedef struct{
  unsigned short inum; // 2 bytes
  char name[DIRSIZE];  // 14 bytes
}dirent;

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


// abstract function

FILE *fp = NULL;

void read(int offset, int size, void *data){
  //here goes file simulation
  assert(fp);

  fseek(fp, offset, SEEK_SET);
  fread(data, size, 1, fp);

}

void write(int offset, int size, const void *data){
  //here goes file simulation
  assert(fp);

  fseek(fp, offset, SEEK_SET);
  fwrite(data, size, 1, fp);

  if (size < BLOCK_SIZE){
    uchar *spare = (uchar*)malloc(BLOCK_SIZE - size);
    memset(spare, 0, BLOCK_SIZE - size);
    fwrite(spare, BLOCK_SIZE - size, 1, fp);
  }

}


// file system functions
/*
inline uint index_first_zero(uchar bin){
  uint pos = 0;
  __asm__("bsfl %1, %0\n\t"
          "jne 1f\n\t"
          "movl $32, %0\n"
          "1:"
          :"=r"(pos)
          :"r"(~(bin)));
  if(pos > 7)
    return 8;
  return (uint)pos;
}
*/
uint index_first_zero(uchar bin){
  uchar r = (bin+1) & (~bin);
  uint pos = 0;
  for(; ((r >> pos ) & 0x1) == 0;pos++);
  return pos;
}

uint uabs(int num) { return (uint)(num < 0 ? -num : num); }

uint balloc(sb_in_mem *sb){
  uint size_bitmap = sb->nblocks / 8 + (sb->nblocks % 8 == 0 ? 0 : 1);
  for(uchar *pBitmap = sb->pBitmap; pBitmap < sb->pBitmap + size_bitmap; pBitmap++)
    if(*pBitmap != 0xFF){
      uint bp =  pBitmap - sb->pBitmap;
      uint index = index_first_zero(*pBitmap);
      INFO(2, "[balloc] Found free block(%d) in bitmap %d",bp*8+index, bp);
      *pBitmap = *pBitmap |( 0x1 << index);
      sb->pBitmap[bp] = *pBitmap;
      // update sbim to sb in disk is not my business!
      INFO(2, "[balloc] Allocated! bitmap is now: 0x%.2x", sb->pBitmap[bp]);
      return bp+index;
    }
  // panic("no allocatable block");
  return sb->nblocks; // not found;
}

void bfree(sb_in_mem *sb, uint b){
  uint index = b/8 +(((b % BPB == 0 ) || (b < 8)) ? 0 : 1);
  uchar pBitmap = sb->pBitmap[index];
  uchar n = 1 << (b % 8);
  INFO(2, "[bfree] Got index like %d, bitmap like 0x%.2x (%d)", b % 8, pBitmap,index);
  if(!(pBitmap & n ))
    return; // panic("block already free");
  pBitmap ^= n;
  sb->pBitmap[index] = pBitmap;
  INFO(2, "[bfree] Found unfree block(%d) in bitmap %d", b, index);
  INFO(2, "[bfree] Freed! bitmap is now: 0x%.2x", sb->pBitmap[index]);
}

uint ialloc(sb_in_mem *sb, uint type){
  return 0;
}

void ifree(sb_in_mem *sb, uint i){
  ;
}


void mkfs(){

  uint size_superblock = BLOCK_SIZE; // sb actually only have 44bytes
  uint size_inodes = INODE_COUNT * sizeof(inode);
  uint size_inodebm = INODE_BITMAP_SIZE;
  uint num_blocks = (TEST_FILE_SIZE / BLOCK_SIZE);
  uint size_bitmap = num_blocks / 8 + (num_blocks % 8 == 0? 0 : 1);
  uint size_journal = 0; // we currently have no journal

  uint offset_inodes = BLOCK_SIZE;
  uint offset_inodebm = BLOCK_SIZE * ( offset_inodes / BLOCK_SIZE + INODE_BLOCKS);
  uint offset_bitmap =
      BLOCK_SIZE * (offset_inodebm / BLOCK_SIZE + size_inodebm / BLOCK_SIZE +
                    (size_inodebm % BLOCK_SIZE == 0 ? 0 : 1));
  uint offset_data =
      BLOCK_SIZE * (offset_bitmap / BLOCK_SIZE + size_bitmap / BLOCK_SIZE +
                    (size_bitmap % BLOCK_SIZE == 0 ? 0 : 1));

  INFO(1, "Size: inodes %d; inodebm %d; bitmap %d; data %d", size_inodes, size_inodebm, size_bitmap, TEST_FILE_SIZE);
  INFO(1, "Offset: inodes %d; inodebm %d; bitmap %d; data %d", offset_inodes, offset_inodebm, offset_bitmap, offset_data);


  assert(offset_inodebm % BLOCK_SIZE == 0);
  assert(offset_inodebm >= offset_inodes + size_inodes);
  assert(offset_bitmap % BLOCK_SIZE == 0);
  assert(offset_bitmap >= offset_inodebm + size_inodebm);
  assert(offset_data % BLOCK_SIZE == 0);
  assert(offset_data >= offset_bitmap + size_bitmap);

  superblock sb;
  memcpy((void*)sb.magic, "OMOCHAFS", 8);
  sb.size = TEST_FILE_SIZE;
  sb.ninodes = INODE_COUNT;
  sb.nblocks = num_blocks;
  sb.njournal = 0;
  sb.offset_inodes = offset_inodes;
  sb.offset_inodebm = offset_inodebm;
  sb.offset_bitmap = offset_bitmap;
  sb.offset_data = offset_data;
  sb.offset_journal = TEST_FILE_SIZE;

  write(0, sizeof(superblock), &sb);
  inode empty_inode_a_block[IPB];
  memset(empty_inode_a_block, 0, sizeof(inode) * IPB);
  for (uint addr = offset_inodes; addr < offset_inodebm; addr += BLOCK_SIZE)
    write(addr, BLOCK_SIZE, empty_inode_a_block);
  INFO(1, "Write all %d blocks for empty inode.", (offset_inodebm - offset_inodes) / BLOCK_SIZE);

  uchar empty_bitmap_a_block[BLOCK_SIZE];
  memset(empty_bitmap_a_block, 0, sizeof(uchar) * BLOCK_SIZE);
  for (uint addr = offset_inodebm; addr < offset_bitmap; addr += BLOCK_SIZE)
    write(addr, BLOCK_SIZE, empty_bitmap_a_block);
  INFO(1, "Write all %d blocks for empty inode block.", (offset_bitmap - offset_inodebm) / BLOCK_SIZE);

  for (uint addr = offset_bitmap; addr < offset_data; addr += BLOCK_SIZE)
    write(addr, BLOCK_SIZE, empty_bitmap_a_block);
  INFO(1, "Write all %d blocks for empty block bitmap.", (offset_data - offset_bitmap) / BLOCK_SIZE);

  const char *test_data = "test data";
  write(offset_data, 10, test_data);
  INFO(1, "Test data is Okey at 0x%x", offset_data);

  sb_in_mem sbim;
  memset(&sbim, 0, sizeof(sb_in_mem));
  memcpy(&sbim, &sb, sizeof(superblock));
  sbim.pInode_Bitmap = (uchar*)malloc(size_inodebm);
  sbim.pBitmap = (uchar*)malloc(size_bitmap);

  read(sbim.offset_inodebm, size_inodebm, sbim.pInode_Bitmap);
  read(sbim.offset_bitmap, size_bitmap, sbim.pBitmap);

  INFO(1, "Superblock in memory copied and initialized!");

  uint root_dir = balloc(&sbim);

  for(uint i = 0; i < 4; i++)
    balloc(&sbim);

  bfree(&sbim,1);
  bfree(&sbim,2);
  bfree(&sbim,0);

  for(uint i = 0; i < 5; i++)
    balloc(&sbim);

}

// test functions
int main(void){
  fp = fopen(TEST_FILE_NAME, "r+");
  INFO(0, "Opened File for simulation.");
  INFO(0,"Sizeof int, short, char is: %ld, %ld, %ld", sizeof(int), sizeof(short), sizeof(char));
  INFO(0,"We got max file size like: %ld bytes, as %ld MB", MAX_FILE_SIZE, MAX_FILE_SIZE/1024/1024);
  INFO(0,"We got size of inode: %ld, so IPB : %ld", sizeof(inode), IPB);
  INFO(0,"We got size of superblock: %ld\nWe got size of dirent: %ld", sizeof(superblock), sizeof(dirent));
  INFO(0,"\nWe got totally %ld blocks for inodes. %ld Bytes (%ld KB). \nAnd inode bitmap uses %ld bytes (%ld KB)",
         INODE_BLOCKS,INODE_BLOCKS * BLOCK_SIZE,INODE_BLOCKS*BLOCK_SIZE/1024, INODE_BITMAP_SIZE, INODE_BITMAP_SIZE / 1024);
  INFO(0,"Persume that we have %s part, so we have to use %ld bytes (%ld KB) to store Bitmap", TEST_FILE_SIZE_CHAR, (long)TEST_FILE_SIZE / BLOCK_SIZE / 8, (long)TEST_FILE_SIZE / BLOCK_SIZE / 8 / 1024);
  long free_space = (long)TEST_FILE_SIZE - ((long)TEST_FILE_SIZE / BLOCK_SIZE / 8) - sizeof(superblock) - (INODE_BLOCKS * BLOCK_SIZE) - (INODE_BITMAP_SIZE) - 0; // we have no journal now
  INFO(0,"So we got %ld bytes (%ld KB, %ld MB) free space to use.", free_space, free_space / 1024, free_space / 1024 / 1024);

  INFO(0,"\nmkfs...");
  mkfs();
  INFO(0,"done!");
  fclose(fp);
  INFO(0,"File Closed.");
  return 0;
}
