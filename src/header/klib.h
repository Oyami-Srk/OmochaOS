#ifndef __OMOCHA_OS__KLIB_H__
#define __OMOCHA_OS__KLIB_H__

#define PUBLIC

PUBLIC char *itoa(int value, char *str, int base);
PUBLIC volatile char *pDisp;
PUBLIC void write_string( int colour, char *string );
PUBLIC void clr_scr(void);


#endif
