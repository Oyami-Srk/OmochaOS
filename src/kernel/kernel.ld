/* link script for OmochaOS's core */
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS {
    .multiboot : AT(0x0) { *(.multiboot) }

    . = 0x80100000;

    .text : AT(ADDR(.text) - 0x80000000) {
        *(.text .stub .text.*)
    }

    . = ALIGN(0x1000);
    
    .rodata : {
        *(.rodata .rodata.*)
    }

    . = ALIGN(0x1000);

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    PROVIDE(KERN_END = .);
}
