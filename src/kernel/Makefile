# GNU Makefile
# src/kernel/

.PHONY: all

KERNEL_BUILD = $(BUILD)/kern
LIB_BUILD = $(BUILD)/lib
KERNEL_LD_FLAGS = -m elf_i386 -N

SRCS := $(wildcard *.c) $(wildcard *.asm)

OBJS := $(patsubst %, $(KERNEL_BUILD)/%.o, $(basename $(SRCS)))
LIB_OBJS := $(wildcard $(LIB_BUILD)/*.o)

all: pre $(KERNEL_BUILD)/kernel.out
	@echo "Kernel is built"

$(KERNEL_BUILD)/kernel.out: $(OBJS) kernel.ld
	$(LD) $(KERNEL_LD_FLAGS) -T kernel.ld -o $@ $(KERNEL_BUILD)/start.o $(filter-out $(KERNEL_BUILD)/start.o, $(OBJS)) $(LIB_OBJS)
	$(OBJDUMP) -S $(KERNEL_BUILD)/kernel.out > $(KERNEL_BUILD)/kernel.disasm

$(KERNEL_BUILD)/%.o: %.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(HEADER)

$(KERNEL_BUILD)/%.o: %.asm
	$(ASM) -f elf $< -I $(HEADER)/ -o $@

$(KERNEL_BUILD)/%.d: %.c
	@set -e; rm -f $@; $(CC) -MM $< -I$(HEADER)  > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
  rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -rf $(KERNEL_BUILD)/*.d $(KERNEL_BUILD)/*.d.*
	rm -rf $(KERNEL_BUILD)/*.lds

pre tools/generate_vector.py:
	mkdir -p $(KERNEL_BUILD)
	$(PY) tools/generate_vector.py > vector_table.asm
