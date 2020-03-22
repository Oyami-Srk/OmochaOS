#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "generic/typedefs.h"

char *itoa(unsigned int value, char *str, int base);
int   vsprintf(char *buf, const char *fmt, va_list args);
int   sprintf(char *buf, const char *fmt, ...);
void  delay_ms(uint ms);

#ifdef __DEBUG__
#define panic(str) panic_proto(#str, __FILE__, __BASE_FILE__, __LINE__)
#define assert(exp)                                                            \
    if (!(exp))                                                                \
    panic(assert exp)

extern void panic_proto(const char *str, const char *s_fn, const char *b_fn,
                        const int ln);
#else
#define panic(str)
#define assert(exp)
#endif

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define NULL      0

#endif // __STDLIB_H__
