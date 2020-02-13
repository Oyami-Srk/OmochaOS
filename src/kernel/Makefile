# GNU Makefile
# src/kernel

KERNEL_BUILD 	= $(BUILD)/kernel
KERNEL_LD_FLAGS = -m elf_i386 -N
KERNEL_HEADER 	= $(shell pwd)/header
SRC_DIRS = core driver lib

.PHONY: all
all: pre-build kernel

.PHONY: create_dirs
create_dirs:
	@mkdir -p $(KERNEL_BUILD)
	@mkdir -p $(foreach n, $(SRC_DIRS), $(KERNEL_BUILD)/$(n))

.PHONY: pre-build
pre-build: create_dirs generate_sources

OBJS = $(foreach n, $(SRC_DIRS), $(patsubst %, $(KERNEL_BUILD)/$(n)/%.o, $(basename $(notdir $(wildcard $(n)/*.c) $(wildcard $(n)/*.asm)))))

$(KERNEL_BUILD)/%.o: %.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(KERNEL_HEADER)

$(KERNEL_BUILD)/%.o: %.asm
	$(ASM) -f elf $< -I $(KERNEL_HEADER)/ -I $(KERNEL_BUILD)/ -o $@

$(KERNEL_BUILD)/%.d: %.c
	@set -e; rm -f $@; $(CC) -MM $< -I$(KERNEL_HEADER)  > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


$(KERNEL_BUILD)/kernel.out: $(OBJS) kernel.ld
	$(LD) $(KERNEL_LD_FLAGS) -T kernel.ld -o $@ $(OBJS)

kernel: $(KERNEL_BUILD)/kernel.out
	@echo Kernel is built.

ifneq ($(filter pre-build,$(MAKECMDGOALS)),pre-build)
-include $(OBJS:.o=.d)
endif

.PHONY: generate_sources
generate_sources: $(KERNEL_BUILD)/multiboot.i

$(KERNEL_BUILD)/multiboot.i: header/core/multiboot.h
	@echo "; auto generated code, DO NOT MODIFIY." > $@
	$(CC) -E -dM -I$(KERNEL_HEADER) $(CC_FLAGS) $^ | awk -F' ' 'substr($$2,0,1)!="_" {gsub(/#/,"%"); print}' >> $@
