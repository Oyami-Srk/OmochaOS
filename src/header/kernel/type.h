#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef uint u32;
typedef ushort u16;
typedef uchar u8;
typedef u8 ubyte;
typedef uint BOOL;
typedef uint size_t;

#define TRUE 1
#define FALSE 0
#define NULL 0

struct __list {
  void *data;
  struct __list *next;
};

typedef struct __list list;

typedef char *va_list;

#endif // __TYPE_H__