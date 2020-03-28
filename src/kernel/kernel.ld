/* link script for multiboot */
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS {
    _KERN_BASE   = 0x80000000; /* notice changing this is only affecting linker script */
    _KERN_OFFSET = 0x100000;
    . = _KERN_BASE + _KERN_OFFSET;
    _KERN_VSTART = .;
    _KERN_PSTART = . - _KERN_BASE;

    .multiboot : AT(0x0) {
        *(.multiboot)
    }

    .text ALIGN(0x1000) : AT(ADDR(.text) - _KERN_BASE) {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - _KERN_BASE) {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    .stab : { /* debug infomation */
        *(.stab)
        BYTE(0)
    }

    .stabstr : {
        *(.stabstr)
        BYTE(0)
    }

    .data ALIGN(0x1000) : AT(ADDR(.data) - _KERN_BASE){
        *(.data)
    }

    .bss : AT(ADDR(.bss) - _KERN_BASE) {
        _KERN_BSS_START = .;
        *(.bss)
        _KERN_BSS_END   = .;
    }
    
    _KERN_VEND = .;
    _KERN_PEND = . - _KERN_BASE;
}
