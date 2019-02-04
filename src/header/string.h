#ifndef __OMOCHA_OS__STRING_H__
#define __OMOCHA_OS__STRING_H__

#define PUBLIC

PUBLIC void* memcpy(void *dst, const void *src, int size);
PUBLIC void* memset(void *dst, char ch, int size);
PUBLIC int   strlen(char *str);

#endif

