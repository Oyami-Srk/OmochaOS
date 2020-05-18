#ifndef __STRING_H__
#define __STRING_H__

char *strcpy(char *dst, char *src);
int   strlen(const char *s);
void *memcpy(void *dst, const void *src, int size);
void *memset(void *dst, char ch, int size);
int   strcmp(const char *cs, const char *ct);
int   memcmp(const char *cs, const char *ct, int count);

#endif // __STRING_H__
