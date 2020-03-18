#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "generic/typedefs.h"

char *itoa(unsigned int value, char *str, int base);
int   vsprintf(char *buf, const char *fmt, va_list args);
int   sprintf(char *buf, const char *fmt, ...);

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define NULL      0

#endif // __STDLIB_H__
