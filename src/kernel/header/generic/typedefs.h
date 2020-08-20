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
    uint ready : 1;
    uint type : 31;
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

typedef struct {
    u16  gs, pd1, fs, pd2, es, pd3, ds, pd4;
    u32  edi, esi, ebp, oesp, ebx, edx, ecx, eax;
    uint trap_no;
    uint err_code;
    u32  eip;
    u16  cs, pd5;
    u32  eflags, esp;
    u16  ss, pd6;
} __attribute__((packed)) stack_frame;

struct tss {
    u32 backlink;
    u32 esp0, ss0, esp1, ss1, esp2, ss2, cr3, eip, flags, eax, ecx, edx, ebx,
        esp, ebp, esi, edi, es, cs, ss, ds, fs, gs, ldt;
    u16 trap;
    u16 iobase; /* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
                // char iomap[2];
} __attribute__((packed));

typedef unsigned int pte_t; // page table entity
typedef unsigned int pde_t; // page dir entity
typedef unsigned int pid_t;

struct prog_info {
    uint  program_size;  // from text to program break
    char *program_break; // virtual addr
    char *image_start;   // virtual addr
    char *break_page_end;
};

struct __process {
    stack_frame stack;
    pde_t *     page_dir;
    uint        status;
    pid_t       pid;
    pid_t       parent_pid;
    char        name[16];
    char *      pstack;
    size_t      pstack_size;
    uint        exit_status;

    message *p_msg;
    // quene is :
    // quene_head_sending_to_this_process->proc.quene_body->.quene_body->....
    struct __process *quene_head_sending_to_this_process;
    struct __process *quene_body;
    struct __process *next;
    struct prog_info *prog_info;
} __attribute__((aligned(32)));

typedef struct __process process;

// typedefs of function pointers
typedef int (*init_fp)(void *); // void * means struct core_env in environment.h
typedef void (*interrupt_func)(int);

#endif // __TYPEDEFS_H__
