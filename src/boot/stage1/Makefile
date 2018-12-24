# GNU Makefile
# src/boot/stage1

.PHONY: all

BOOT_BUILD	=	$(BUILD)/boot

BOOT_ASM_INC := protect
BOOT_C_HEADER := asm_x86 elf memory_map

all: pre $(BOOT_BUILD)/boot.bin			# should be less than 440 Bytes

$(BOOT_BUILD)/boot.bin: $(BOOT_BUILD)/boot_asm.o $(BOOT_BUILD)/boot_c.o
	$(LD) -N -e start -Ttext 0x7c00 -o $(BOOT_BUILD)/boot.o $(BOOT_BUILD)/boot_asm.o $(BOOT_BUILD)/boot_c.o
	$(OBJDUMP) -S $(BOOT_BUILD)/boot.o > $(BOOT_BUILD)/boot.disasm # For debug
	$(OBJCOPY) -S -O binary -j .text $(BOOT_BUILD)/boot.o $(BOOT_BUILD)/boot.bin

$(BOOT_BUILD)/boot_asm.o: boot_sector.asm $(foreach inc_file, $(BOOT_ASM_INC), $(wildcard $(BOOT_COMMON_HEADER)/$(inc_file).asm))
	$(ASM) -f elf boot_sector.asm -I $(BOOT_COMMON_HEADER) -o $(BOOT_BUILD)/boot_asm.o

$(BOOT_BUILD)/boot_c.o: boot_sector.c $(foreach inc_file, $(BOOT_C_HEADER), $(wildcard $(BOOT_COMMON_HEADER)/$(inc_file).h))
	$(CC) $(CC-FLAG-BOOT_STAGE1) -I$(BOOT_COMMON_HEADER) boot_sector.c -o $(BOOT_BUILD)/boot_c.o

pre:
	mkdir -p $(BOOT_BUILD)

