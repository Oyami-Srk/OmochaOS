#include "lib/stdlib.h"

char *itoa(unsigned int value, char *str, int base) {
  char *rc;
  char *ptr;
  char *low;
  if (base < 2 || base > 36) {
    *str = '\0';
    return str;
  }
  rc = ptr = str;
  if (value < 0 && base == 10)
    *ptr++ = '-';
  low = ptr;
  do {
    *ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRST"
             "UVWXYZ"[35 + value % base];
    value /= base;
  } while (value);
  *ptr-- = '\0';
  while (low < ptr) {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  return rc;
}