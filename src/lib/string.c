#include "lib/string.h"

// copy from asm.h, so we do not need to include the header

static inline void stosb(void *addr, char data, int size) {
  __asm__ __volatile__("cld; rep stosb"
                       : "=D"(addr), "=c"(size)
                       : "0"(addr), "1"(size), "a"(data)
                       : "memory", "cc");
}

static inline void stosl(void *addr, char data, int size) {
  __asm__ __volatile__("cld; rep stosl"
                       : "=D"(addr), "=c"(size)
                       : "0"(addr), "1"(size), "a"(data)
                       : "memory", "cc");
}

void *memcpy(void *dst, const void *src, int size) {
  const char *s;
  char *d;
  s = src;
  d = dst;
  if (s < d &&
      s + size > d) { // 当src和dst有重叠时从后向前拷贝，避免覆盖产生的错误
    s += size;
    d += size;
    while (size-- > 0)
      *--d = *--s;
  } else
    while (size-- > 0)
      *d++ = *s++;
  return dst;
}

char *strcpy(char *dst, char *src) {
  char *odst;
  if (src < dst)
    while ((*dst++ = *src++) != '\0')
      ;
  else
    memcpy(dst, src, strlen(src) + 1);
  return odst;
}

int strlen(const char *s) {
  const char *eos = s;
  while (*eos++)
    ;
  return (eos - s - 1);
}

void *memset(void *dst, char ch, int size) {
  if ((int)dst % 4 == 0 &&
      size % 4 == 0) { // 当对齐的时候，一口气写入long而不是byte
    ch &= 0xFF;
    stosl(dst, (ch << 24) | (ch << 16) | (ch << 8) | ch, size / 4);
  } else
    stosb(dst, ch, size);
  return dst;
}

int strcmp(const char *cs, const char *ct) {
  unsigned char c1, c2;
  while (1) {
    c1 = *cs++;
    c2 = *ct++;
    if (c1 != c2)
      return c1 < c2 ? -1 : 1;
    if (!c1)
      break;
  }
  return 0;
}