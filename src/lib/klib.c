#include "klib.h"

PUBLIC char *itoa(unsigned int value, char *str, int base){
  char *rc;
  char *ptr;
  char *low;
  if(base < 2 || base > 36){
    *str = '\0';
    return str;
  }
  rc = ptr = str;
  if(value < 0 && base == 10)
    *ptr++ = '-';
  low = ptr;
  do{
    *ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[35 + value % base];
    value /= base;
  }while(value);
  *ptr-- = '\0';
  while(low < ptr) {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  return rc;
}

// for temperary debug usage

PUBLIC void write_string( int colour, char *string ) {
  char ch = *string;
  while (*string != 0) {
    if(ch == '\n'){
      pDisp = (volatile char*)0xB8000 + 80 * 2 * (((int)pDisp - 0xB8000) / 160 + 1);
      ch = *++string;
      continue;
    }
    *pDisp++ = ch;
    ch = *++string;
    *pDisp++ = colour;
  }
}

PUBLIC void clr_scr(void){
  volatile char *p = (volatile char*)0xB8000;
  pDisp = (volatile char*)0xB8000;
  for(int i = 0; i < 80 * 25; i++){
    *p++ = 0x00;
    *p++ = 0x00;
  }
}

volatile char *pDisp = (volatile char*)0xB8000;

PUBLIC int ckprintf(int color, const char *fmt, ...){
  int i;
  char buf[256];
  va_list arg = (va_list)((char*)(&fmt) + 4);
  i = vsprintf(buf, fmt, arg);
  buf[i] = 0;
  write_string(color, buf);
  return i;
}

PUBLIC int kprintf(const char *fmt, ...){
  int i;
  char buf[256];
  va_list arg = (va_list)((char*)(&fmt) + 4);
  i = vsprintf(buf, fmt, arg);
  buf[i] = 0;
  write_string(0x0F, buf);
  return i;
}


PUBLIC void panic_proto(const char* str, const char* s_fn, const char* b_fn, const int ln){
  __asm__ __volatile__("cli"); // close interrupt
  pDisp = (volatile char*)0xB8000;
  ckprintf(0x1C, "!PANIC! %s :: File -> %s, BaseFile -> %s, Line -> %d", str, s_fn, b_fn, ln);
  while(1);
}

PUBLIC int vsprintf(char *buf, const char *fmt, va_list args){
  va_list arg = args;
  int m;

  char inner_buf[1024]; // hardcode is not good
  char cs;
  int align;

  char *p;
  for(p = buf; *fmt; fmt++){
    if(*fmt != '%') {
      *p++ = *fmt;
      continue;
    } else {
      align = 0;
    }
    fmt++;
    if(*fmt == '%'){
      *p++ = *fmt;
      continue;
    } else if (*fmt == '0') {
      cs = '0';
      fmt++;
    } else{
      cs = ' ';
    }

    while (((unsigned char)(*fmt) >= '0') && ((unsigned char)(*fmt) <= '9')) {
      align *= 10;
      align += *fmt - '0';
      fmt++;
    }

    char *q = inner_buf;
    memset((void*)q, 0, sizeof(inner_buf));

    switch (*fmt) {
    case 'c':
      *q++ = *((char*)arg);
      arg += 4;
      break;
    case 'x':
      m = *((int*)arg);
      itoa(m, q, 16);
      arg += 4;
      break;
    case 'd':
      m = *((int*)arg);
      if (m < 0){
        m = m * (-1);
        *q++ = '-';
      }
      itoa(m, q, 10);
      arg += 4;
      break;
    case 's':
      strcpy(q, (*((char**)arg)));
      q += strlen(*((char**)arg));
      arg += 4;
    default:
      break;
    }

    for(int k = 0; k < ((align > strlen(inner_buf)) ? (align - strlen(inner_buf)) : 0); k++)
      *p++ = cs;
    q = inner_buf;
    while(*q)
      *p++ = *q++;

  }

  *p = 0;
  return (p - buf);
}

PUBLIC int sprintf(char *buf, const char *fmt, ...){
  va_list arg = (va_list)((char*)(&fmt) + 4);
  return vsprintf(buf, fmt, arg);
}


// syscalls

uint get_ticks(){
  volatile uint beats;
  __asm__("movl $0, %%eax\n\t"
          "int $0xE9\n\t"
          "movl %0, %%eax"
          :"=r"(beats)
          :
          :"memory");
  return beats;
}


uint test_parm(int v2, int v3, int v4){
  volatile uint rv;
  __asm__ __volatile__("movl %%ecx, %%ecx\n\t"::"c"(v2));
  __asm__ __volatile__("movl %%ebx, %%ebx\n\t"::"b"(v3));
  __asm__ __volatile__("movl %%edx, %%edx\n\t"::"d"(v4));
  __asm__ __volatile__("movl $1, %%eax\n\t"
                       "int $0xE9\n\t"
                       "movl %0, %%eax"
                       :"=r"(rv)
                       :
                       :"memory");
  return rv;
}
