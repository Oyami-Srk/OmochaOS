#include "lib/stdlib.h"
#include "const.h"
#include "kernel/type.h"
#include "lib/asm.h"
#include "lib/string.h"
#include "syscall/syscall.h"

#include "driver/vga.h"

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

int vsprintf(char *buf, const char *fmt, va_list args) {
  va_list arg = args;
  int m;

  char inner_buf[1024]; // hardcode is not good
  char cs;
  int align;

  char *p;
  for (p = buf; *fmt; fmt++) {
    if (*fmt != '%') {
      *p++ = *fmt;
      continue;
    } else {
      align = 0;
    }
    fmt++;
    if (*fmt == '%') {
      *p++ = *fmt;
      continue;
    } else if (*fmt == '0') {
      cs = '0';
      fmt++;
    } else {
      cs = ' ';
    }

    while (((unsigned char)(*fmt) >= '0') && ((unsigned char)(*fmt) <= '9')) {
      align *= 10;
      align += *fmt - '0';
      fmt++;
    }

    char *q = inner_buf;
    memset((void *)q, 0, sizeof(inner_buf));

    switch (*fmt) {
    case 'c':
      *q++ = *((char *)arg);
      arg += 4;
      break;
    case 'x':
      m = *((int *)arg);
      itoa(m, q, 16);
      arg += 4;
      break;
    case 'd':
      m = *((int *)arg);
      if (m < 0) {
        m = m * (-1);
        *q++ = '-';
      }
      itoa(m, q, 10);
      arg += 4;
      break;
    case 's':
      strcpy(q, (*((char **)arg)));
      q += strlen(*((char **)arg));
      arg += 4;
    default:
      break;
    }

    for (int k = 0;
         k < ((align > strlen(inner_buf)) ? (align - strlen(inner_buf)) : 0);
         k++)
      *p++ = cs;
    q = inner_buf;
    while (*q)
      *p++ = *q++;
  }

  *p = 0;
  return (p - buf);
}

int sprintf(char *buf, const char *fmt, ...) {
  va_list arg = (va_list)((char *)(&fmt) + 4);
  return vsprintf(buf, fmt, arg);
}

void delay_ms(uint ms) {
  uint beats_begin = get_ticks_msg();
  while (((get_ticks_msg() - beats_begin) * 1000 / SYSTEM_CLOCK) < ms)
    ;
}

void panic_proto(const char *str, const char *s_fn, const char *b_fn,
                 const int ln) {
  asm volatile("cli"); // close interrupt
  VGA_write_color_string_to_vm(0, COLOR(BLUE, BLACK),
                               "                    "
                               "                    "
                               "                    "
                               "                   ");
  char buf[4];
  char *p_str = (char *)str;
  uint i = 0;
  i = VGA_write_color_string_to_vm(i, COLOR(BLUE, WHITE), "[PANIC] ");
  i = VGA_write_color_string_to_vm(i, COLOR(BLUE, WHITE), p_str);
  i = VGA_write_color_string_to_vm(i, COLOR(BLUE, WHITE), " in line ");
  itoa(ln, buf, 10);
  i = VGA_write_color_string_to_vm(i, COLOR(BLUE, WHITE), buf);
  i = VGA_write_color_string_to_vm(i, COLOR(BLUE, WHITE), " of ");
  i = VGA_write_color_string_to_vm(i, COLOR(BLUE, WHITE), s_fn);
  i = VGA_write_color_string_to_vm(i, COLOR(BLUE, WHITE), " based on ");
  i = VGA_write_color_string_to_vm(i, COLOR(BLUE, WHITE), b_fn);
  magic_break();
  while (1)
    ;
}