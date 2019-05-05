#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "kernel/type.h"

char *itoa(unsigned int value, char *str, int base);
int sprintf(char *buf, const char *fmt, ...);
void delay_ms(uint);

#endif // __STDLIB_H__