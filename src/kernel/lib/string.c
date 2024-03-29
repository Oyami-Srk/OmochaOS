#include <generic/asm.h>
#include <lib/string.h>

void *memcpy(void *dst, const void *src, int size) {
    const char *s;
    char *      d;
    s = src;
    d = dst;
    if (s < d &&
        s + size > d) { // 当src和dst有重叠时从后向前拷贝，避免覆盖产生的错误
        s += size;
        d += size;
        while (size-- > 0)
            *--d = *--s;
    } else
        while (size-- > 0)
            *d++ = *s++;
    return dst;
}

char *strcpy(char *dst, char *src) {
    char *odst;
    if (src < dst)
        while ((*dst++ = *src++) != '\0')
            ;
    else
        memcpy(dst, src, strlen(src) + 1);
    return odst;
}

int strlen(const char *s) {
    const char *eos = s;
    while (*eos++)
        ;
    return (eos - s - 1);
}

void *memset(void *dst, char ch, int size) {
    if ((int)dst % 4 == 0 &&
        size % 4 == 0) { // 当对齐的时候，一口气写入long而不是byte
        ch &= 0xFF;
        unsigned long data;
        char         *pdata = (char *)&data;
        pdata[0] = pdata[1] = pdata[2] = pdata[3] = ch;
        stosl(dst, data, size / 4);
    } else
        stosb(dst, ch, size);
    return dst;
}

int strcmp(const char *cs, const char *ct) {
    unsigned char *c1 = (unsigned char *)cs;
    unsigned char *c2 = (unsigned char *)ct;
    while (1) {
        if (*c1 != *c2)
            return *c1 < *c2 ? -1 : 1;
        if (!*c1)
            break;
        c1++;
        c2++;
    }
    return 0;
}

int memcmp(const char *cs, const char *ct, int count) {
    unsigned char *c1 = (unsigned char *)cs;
    unsigned char *c2 = (unsigned char *)ct;
    while (count-- > 0) {
        if (*c1 != *c2)
            return *c1 < *c2 ? -1 : 1;
        c1++;
        c2++;
    }
    return 0;
}