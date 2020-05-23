# GNU Makefile
# src/kernel

KERNEL_BUILD 	= $(BUILD)/kernel
KERNEL_LD_FLAGS = -m elf_i386 -N
KERNEL_HEADER 	= $(shell pwd)/header
SRC_DIRS = core driver lib modules

.PHONY: all
all: pre-build kernel

.PHONY: create_dirs
create_dirs:
	@$(MKDIR) -p $(KERNEL_BUILD)
	@$(MKDIR) -p $(foreach n, $(SRC_DIRS), $(KERNEL_BUILD)/$(n))
	@$(MKDIR) -p $(foreach n, $(MODULES_DIRS), $(KERNEL_BUILD)/$(n))

.PHONY: pre-build
pre-build: create_dirs generate_sources

OBJS = $(foreach n, $(SRC_DIRS), $(patsubst %, $(KERNEL_BUILD)/$(n)/%.o, $(basename $(notdir $(wildcard $(n)/*.c) $(wildcard $(n)/*.asm)))))
MODULES_DIRS = $(sort $(dir $(wildcard modules/*/)))
MODULES_SRCS = $(foreach n, $(MODULES_DIRS), $(wildcard $(n)*.c))
MODULES_OBJS = $(foreach n, $(MODULES_SRCS), $(patsubst %, $(KERNEL_BUILD)/%.o, $(basename $(n))))
OBJS += $(MODULES_OBJS)

$(KERNEL_BUILD)/%.o: %.c
	# $(CC) -o $@ -c $< $(CC_FLAGS) -I$(KERNEL_HEADER)
	$(CC) -MMD -c -o $@ $< $(CC_FLAGS) -I $(KERNEL_HEADER)/

$(KERNEL_BUILD)/%.o: %.asm
	$(ASM) -f elf $< -I $(KERNEL_HEADER)/ -I $(KERNEL_BUILD)/ -o $@

#$(KERNEL_BUILD)/%.d: %.c
	# @set -e; rm -f $@; $(CC) -MM $< -I$(KERNEL_HEADER) > $@.$$$$; \
	# $(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	# $(RM) -f $@.$$$$
	# $(CC) -MMD -c -o $@ $< $(CC_FLAGS) -I $(KERNEL_HEADER)/


$(KERNEL_BUILD)/kernel.out: $(OBJS) kernel.ld $(KERNEL_BUILD)/init.out.o
	$(LD) $(KERNEL_LD_FLAGS) -T kernel.ld -o $@ $(OBJS) $(KERNEL_BUILD)/init.out.o

LIBOS_OBJS = lib/string lib/rbtree lib/stdlib lib/libos
LIBOS_OBJS := $(foreach o,$(LIBOS_OBJS),$(KERNEL_BUILD)/$(o).o)
LIBOS = $(BUILD)/libos.a

$(LIBOS): $(LIBOS_OBJS)
	$(AR) -rcs $@ $(LIBOS_OBJS)

$(KERNEL_BUILD)/init.o: init/init.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(KERNEL_HEADER)

$(KERNEL_BUILD)/init.out: $(KERNEL_BUILD)/init.o $(LIBOS)
	$(LD) -m elf_i386 -e init -Ttext 0x0 -o $@ $^

$(KERNEL_BUILD)/init.out.o: $(KERNEL_BUILD)/init.out init/package.S
	$(CC) -m32 -D__BIN_PATH__='"$(KERNEL_BUILD)/init.out"' init/package.S -c -o $@

kernel: $(KERNEL_BUILD)/kernel.out
	@echo Kernel is built.

-include $(OBJS:.o=.d)

.PHONY: generate_sources
generate_sources: $(KERNEL_BUILD)/multiboot.i header/modules/modules.h

$(KERNEL_BUILD)/multiboot.i: header/core/multiboot.h
	@echo "; auto generated code, DO NOT MODIFIY." > $@
	$(CC) -E -dM -I$(KERNEL_HEADER) $(CC_FLAGS) $^ | $(AWK) -F' ' 'substr($$2,0,1)!="_" {gsub(/#/,"%"); print}' >> $@

header/modules/modules.h: $(wildcard modules/*/*.yaml) modules/module_parser.py
	$(PY3) modules/module_parser.py -m modules -a $@ --dryrun
	$(PY3) modules/module_parser.py -m modules -a $@
