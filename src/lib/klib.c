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
