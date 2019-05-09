#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "kernel/type.h"

char *itoa(unsigned int value, char *str, int base);
int sprintf(char *buf, const char *fmt, ...);
void delay_ms(uint);

// for debug
#define panic(str) panic_proto(#str, __FILE__, __BASE_FILE__, __LINE__)
#define assert(exp)                                                            \
  if (!(exp))                                                                  \
  panic(assert exp)
void panic_proto(const char *str, const char *s_fn, const char *b_fn,
                 const int ln);

#endif // __STDLIB_H__