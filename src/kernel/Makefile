# GNU Makefile
# src/kernel

.PHONY: all

KERNEL_BUILD 	= $(BUILD)/kernel
KERNEL_LD_FLAGS = -m elf_i386 -N
KERNEL_HEADER 	= $(shell pwd)/header

all: preprocess kernel
	@echo "Kernel is built."


.PHONY: preprocess
preprocess:
	mkdir -p $(KERNEL_BUILD)
	mkdir -p $(KERNEL_BUILD)/core

# Core

CORE_SRCS = $(wildcard core/*.c) $(wildcard core/*.asm)
CORE_OBJS = $(patsubst %, $(KERNEL_BUILD)/core/%.o, $(basename $(notdir $(CORE_SRCS))))

core: $(CORE_OBJS)
	$(LD) -r -s -o $(KERNEL_BUILD)/core.o $(CORE_OBJS)
	@echo "Core is built."

$(KERNEL_BUILD)/core/%.o: core/%.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(KERNEL_HEADER)

$(KERNEL_BUILD)/core/%.o: core/%.asm
	$(ASM) -f elf $< -I $(KERNEL_HEADER)/ -o $@

$(KERNEL_BUILD)/core/%.d: core/%.c
	@set -e; rm -f $@; $(CC) -MM $< -I$(KERNEL_HEADER)  > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(CORE_OBJS:.o=.d)

.PHONY: clean_core
clean_core:
	rm -rf $(KERNEL_BUILD)/core/*.d $(KERNEL_BUILD)/core/*.d.* $(KERNEL_BUILD)/core/*.lds

# Kernel

OBJS = $(KERNEL_BUILD)/core.o

kernel: core
	$(LD) $(KERNEL_LD_FLAGS) -T kernel.ld -o $(KERNEL_BUILD)/kernel.out $(OBJS)

