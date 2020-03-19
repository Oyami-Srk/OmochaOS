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
#define TRUE  1
#define FALSE 0

typedef uint size_t;

struct __proto_list__ {
    void *                 data;
    struct __proto_list__ *next;
};

typedef struct __proto_list__ list;

typedef char *va_list;

typedef struct __message {
    uint sender;
    uint receiver;
    struct {
        uint ready : 1;
        uint type : 7;
    } type;
    uint major;
    union {
        struct {
            uint d1;
            uint d2;
            uint d3;
            uint d4;
        } uint_arr;
        struct {
            uint d1;
            char d2[12];
        } uint_b12;
        char b16[16];
    } data;
} __attribute__((packed)) message;

#endif // __TYPEDEFS_H__
