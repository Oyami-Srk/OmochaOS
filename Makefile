# GNU Makefile
# .

ASM			 	= nasm
CC				= i386-elf-gcc
LD				= i386-elf-ld
OBJDUMP	  = i386-elf-objdump
OBJCOPY   = i386-elf-objcopy
DD				= dd
BOCHS			= bochs
CAT				= cat
SED				= sed
BXIMAGE		= bximage

BUILD			= $(shell pwd)/build

export ASM CC LD OBJDUMP OBJCOPY BUILD DD BOCHS CAT SED BXIMAGE

.PHONY: all
all:
	make -C src

.PHONY: clean
clean:
	rm -rf $(BUILD)/*

.PHONY: boot
boot:
	make -C src/boot boot
