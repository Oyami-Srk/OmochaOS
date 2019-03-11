# GNU Makefile
# src/modules/

.PHONY: all

MODULES_BUILD = $(BUILD)/modules
MODULES_LD_FLAGS = -m elf_i386 -N

SRCS := $(wildcard *.c)

OBJS := $(patsubst %, $(MODULES_BUILD)/%.o, $(basename $(SRCS)))

all: pre $(OBJS)
	@echo "Modules are built"

$(MODULES_BUILD)/%.o: %.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(HEADER)

$(MODULES_BUILD)/%.d: %.c
	@set -e; rm -f $@; $(CC) -MM $< -I$(HEADER)  > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
  rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -rf $(MODULES_BUILD)/*.d $(MODULES_BUILD)/*.d.*
	rm -rf $(MODULES_BUILD)/*.lds

pre:
	mkdir -p $(MODULES_BUILD)

