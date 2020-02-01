#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef uint   u32;
typedef ushort u16;
typedef uchar  u8;

typedef uchar ubyte;
typedef uint  BOOL;

typedef uint size_t;

struct __proto_list__ {
    void *                 data;
    struct __proto_list__ *next;
};

typedef struct __proto_list__ list;

typedef char *va_list;

#endif // __TYPEDEFS_H__
