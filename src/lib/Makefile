# GNU Makefile
# src/lib/

.PHONY: all

LIB_BUILD = $(BUILD)/lib
LIB_LD_FLAGS = -m elf_i386 -N

SRCS := $(wildcard *.c) $(wildcard *.asm)

OBJS := $(patsubst %, $(LIB_BUILD)/%.o, $(basename $(SRCS)))

all: pre $(OBJS)
	@echo "Lib is built"

$(LIB_BUILD)/%.o: %.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(HEADER)

$(LIB_BUILD)/%.o: %.asm
	$(ASM) -f elf $< -I $(HEADER)/ -o $@

$(LIB_BUILD)/%.d: %.c
	@set -e; rm -f $@; $(CC) -MM $< -I$(HEADER)  > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
  rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -rf $(LIB_BUILD)/*.d $(LIB_BUILD)/*.d.*
	rm -rf $(LIB_BUILD)/*.lds

pre:
	mkdir -p $(LIB_BUILD)

