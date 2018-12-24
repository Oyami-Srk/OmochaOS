#ifndef __OMO_ELF_H__
#define __OMO_ELF_H__
// ELF 文件头
// 简单版本，以后可能会从FreeBSD里复制一个elf头文件过来
// 复制自xv6
#define ELF_MAGIC 0x464C457FU

#define ELF_PROG_LOAD 1
#define ELF_PROG_FLAG_EXEC 1
#define ELF_PROG_FLAG_WRITE 2
#define ELF_PROG_FLAG_READ 4

typedef struct{
  unsigned int magic;
  unsigned char elf[12];
  unsigned short type;
  unsigned short machine;
  unsigned int version;
  unsigned int entry;
  unsigned int phoff;
  unsigned int shoff;
  unsigned int flags;
  unsigned short ehsize;
  unsigned short phentsize;
  unsigned short phnum;
  unsigned short shentsize;
  unsigned short shnum;
  unsigned short shstrndx;
}ELF32_Header;

typedef struct{
  unsigned int type;
  unsigned int off;
  unsigned int vaddr;
  unsigned int paddr;
  unsigned int filesz;
  unsigned int memsz;
  unsigned int flags;
  unsigned int align;
}ELF32_Prog;

#endif // __OMO_ELF_H__
