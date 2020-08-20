#ifndef __OS_ELF32_H__
#define __OS_ELF32_H__

// copy from osdev.org

#include <generic/typedefs.h>

typedef u16 Elf32_Half;  // unsigned half word
typedef u32 Elf32_Off;   // unsigned offset
typedef u32 Elf32_Addr;  //  unsigned address
typedef u32 Elf32_Word;  // unsigned word
typedef int Elf32_Sword; // signed word

#define ELF_NIDENT 16

typedef struct {
    u8         e_ident[ELF_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off  e_phoff;
    Elf32_Off  e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Ehdr;

enum Elf_Ident {
    EI_MAG0       = 0, // 0x7F
    EI_MAG1       = 1, // 'E'
    EI_MAG2       = 2, // 'L'
    EI_MAG3       = 3, // 'F'
    EI_CLASS      = 4, // Architecture (32/64)
    EI_DATA       = 5, // Byte Order
    EI_VERSION    = 6, // ELF Version
    EI_OSABI      = 7, // OS Specific
    EI_ABIVERSION = 8, // OS Specific
    EI_PAD        = 9  // Padding
};

#define ELFMAG0 0x7F // e_ident[EI_MAG0]
#define ELFMAG1 'E'  // e_ident[EI_MAG1]
#define ELFMAG2 'L'  // e_ident[EI_MAG2]
#define ELFMAG3 'F'  // e_ident[EI_MAG3]

#define ELFDATA2LSB (1) // Little Endian
#define ELFCLASS32  (1) // 32-bit Architecture

enum Elf_Type {
    ET_NONE = 0, // Unkown Type
    ET_REL  = 1, // Relocatable File
    ET_EXEC = 2  // Executable File
};

#define EM_386     (3) // x86 Machine Type
#define EV_CURRENT (1) // ELF Current Version

typedef struct {
    Elf32_Word p_type;
    Elf32_Off  p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr; // nouse
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
} __attribute__((packed)) Elf32_Phdr;

enum Elf_PType {
    PT_NULL    = 0,
    PT_LOAD    = 1,
    PT_DYNAMIC = 2,
    PT_INTERP  = 3,
    PT_NOTE    = 4,
    PT_SHLIB   = 5,
    PT_PHDR    = 6,
    PT_LOPROC  = 0x70000000,
    PT_HIPROC  = 0x7FFFFFFF
};

enum Elf_PFlay {
    PF_X        = 1,
    PF_W        = 2,
    PF_R        = 4,
    PF_MASKOS   = 0x0FF00000,
    PF_MASKPROC = 0xF0000000,
};

typedef struct {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} __attribute__((packed)) Elf32_Shdr;

#endif // __OS_ELF32_H__
