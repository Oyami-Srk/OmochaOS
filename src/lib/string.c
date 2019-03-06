#include "string.h"
#include "kernel/asm.h"

void* memcpy(void *dst, const void *src, int size){
  const char *s;
  char *d;
  s = src;
  d = dst;
  if(s < d && s + size > d){  // 当src和dst有重叠时从后向前拷贝，避免覆盖产生的错误
    s += size;
    d += size;
    while(size-- > 0)
      *--d = *--s;
  }else
    while(size -- > 0)
      *d++ = *s++;
  return dst;
}

char* strcpy(char *dst, char *src){
  char *odst;
  if(src < dst)
    while((*dst++=*src++) != '\0');
  else
    memcpy(dst, src, strlen(src) + 1);
  return odst;
}

int strlen(const char *s){
  const char *eos = s;
  while(*eos++);
  return (eos - s - 1);
}

void* memset(void *dst, char ch, int size){
  if((int)dst % 4 == 0 && size % 4 == 0){ // 当对齐的时候，一口气写入long而不是byte
    ch &= 0xFF;
    stosl(dst, (ch << 24) | (ch << 16) | (ch << 8) | ch, size / 4);
  } else
    stosb(dst, ch, size);
  return dst;
}
