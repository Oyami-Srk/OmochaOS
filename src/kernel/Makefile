# GNU Makefile
# src/kernel/

.PHONY: all

KERNEL_BUILD = $(BUILD)/kern
KERNEL_LD_FLAGS = -m elf_i386 -N

SRCS := $(wildcard *.c)

OBJS := $(pathsubst %, $(KERNEL_BUILD)/%.o, $(basename $(SRCS)))

all: pre $(KERNEL_BUILD)/kernel.out
	@echo "Kernel is built"

$(KERNEL_BUILD)/kernel.out: $(KERNEL_BUILD)/start.o $(KERNEL_BUILD)/kernel.o $(OBJS) kernel.ld
	$(LD) $(KERNEL_LD_FLAGS) -T kernel.ld -o $@ $^

$(KERNEL_BUILD)/%.o: %.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(HEADER)

$(KERNEL_BUILD)/start.o: start.asm
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

pre:
	mkdir -p $(KERNEL_BUILD)
