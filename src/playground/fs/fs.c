// 用来做实验的，没什么意义的文件

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// tools

#define INFO(indent, fmt, ...) printf("%*s" fmt "\n", (int)indent * 4 , "", ##__VA_ARGS__)

#define TEST_FILE_NAME "test.bin"

// abstract function

void read(int offset, int size, void *data){
  //here goes file simulation
  FILE *fp = fopen(TEST_FILE_NAME, "rb");

  fseek(fp, offset, 0);
  fread(data, size, 1, fp);

  fclose(fp);
}

void write(int offset, int size, const void *data){
  //here goes file simulation
  FILE *fp = fopen(TEST_FILE_NAME, "wb");

  fseek(fp, offset, 0);
  fwrite(data, size, 1, fp);

  fclose(fp);
}

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

#define INODE_COUNT (1024 * 64)
#define INODE_BLOCKS (INODE_COUNT / IPB)

#define INODE_BITMAP_SIZE (INODE_COUNT / 8)

#define TEST_FILE_SIZE (20 * 1024 * 1024)
#define TEST_FILE_SIZE_CHAR "20M"
#define _80MINBYTES (80 * 1024 * 1024)

// file system functions

void mkfs(){

  superblock sb;
  memcpy((void*)sb.magic, "OMOCHAFS", 8);
  sb.size = TEST_FILE_SIZE;
  sb.ninodes = INODE_COUNT;
}

// test functions
int main(void){
  INFO(0,"Sizeof int, short, char is: %ld, %ld, %ld", sizeof(int), sizeof(short), sizeof(char));
  INFO(0,"We got max file size like: %ld bytes, as %ld MB", MAX_FILE_SIZE, MAX_FILE_SIZE/1024/1024);
  INFO(0,"We got size of inode: %ld, so IPB : %ld", sizeof(inode), IPB);
  INFO(0,"We got size of superblock: %ld\nWe got size of dirent: %ld", sizeof(superblock), sizeof(dirent));
  INFO(0,"\nWe got totally %ld blocks for inodes. %ld Bytes (%ld KB). \nAnd inode bitmap uses %d bytes (%d KB)",
         INODE_BLOCKS,INODE_BLOCKS * BLOCK_SIZE,INODE_BLOCKS*BLOCK_SIZE/1024, INODE_BITMAP_SIZE, INODE_BITMAP_SIZE / 1024);
  INFO(0,"Persume that we have %s part, so we have to use %ld bytes (%ld KB) to store Bitmap", TEST_FILE_SIZE_CHAR, (long)TEST_FILE_SIZE / BLOCK_SIZE / 8, (long)TEST_FILE_SIZE / BLOCK_SIZE / 8 / 1024);
  long free_space = (long)TEST_FILE_SIZE - ((long)TEST_FILE_SIZE / BLOCK_SIZE / 8) - sizeof(superblock) - (INODE_BLOCKS * BLOCK_SIZE) - (INODE_BITMAP_SIZE) - 0; // we have no journal now
  INFO(0,"So we got %ld bytes (%ld KB, %ld MB) free space to use.", free_space, free_space / 1024, free_space / 1024 / 1024);

  INFO(0,"\nmkfs...");
  mkfs();
  INFO(0,"done!");
  return 0;
}
