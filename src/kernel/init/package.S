.section .init_code
.global _init_elf_start_
_init_elf_start_:
    .incbin __BIN_PATH__
.global _init_elf_end_
_init_elf_end_: