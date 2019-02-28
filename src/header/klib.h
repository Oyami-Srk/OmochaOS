#ifndef __OMOCHA_OS__KLIB_H__
#define __OMOCHA_OS__KLIB_H__

#include "type.h"

#define PUBLIC

PUBLIC char *itoa(unsigned int value, char *str, int base);
PUBLIC volatile char *pDisp;
PUBLIC void write_string( int colour, char *string );
PUBLIC void clr_scr(void);

#define panic(str) panic_proto(#str, __FILE__, __BASE_FILE__, __LINE__)
#define assert(exp) if(!(exp))panic(assert exp)
#define PANIC_COLOR 0x1F
PUBLIC void panic_proto(const char *str, const char *s_fn, const char *b_fn, const int ln);

PUBLIC int vsprintf(char *buf, const char *fmt, va_list args);
PUBLIC int sprintf(char *buf, const char *fmt, ...);
PUBLIC int kprintf(const char *fmt, ...);
PUBLIC int ckprintf(int color, const char *fmt, ...);

// syscalls
PUBLIC uint get_ticks();
PUBLIC uint test_parm(int v2, int v3, int v4);

#endif
