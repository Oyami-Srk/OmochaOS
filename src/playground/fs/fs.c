// 用来做实验的，没什么意义的文件

#include <stdio.h>
#include <stdlib.h>

// abstract function

void read(int seek, int size, void *data){
  //here goes file simulation
  FILE *fp = fopen("test.bin", "rb");

  fclose(fp);
}

void write(int seek, int size, void *data){
  //here goes file simulation
  FILE *fp = fopen("test.bin", "rb");

  fclose(fp);
}

// file system defination

// file system functions
