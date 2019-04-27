# GNU Makefile
# src/drivers/

.PHONY: all

DRIVERS_BUILD = $(BUILD)/drivers
DRIVERS_LD_FLAGS = -m elf_i386 -N

SRCS := $(wildcard *.c)

OBJS := $(patsubst %, $(DRIVERS_BUILD)/%.o, $(basename $(SRCS)))

all: pre $(OBJS)
	@echo "drivers are built"

$(DRIVERS_BUILD)/%.o: %.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(HEADER)

$(DRIVERS_BUILD)/%.d: %.c
	@set -e; rm -f $@; $(CC) -MM $< -I$(HEADER)  > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
  rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -rf $(DRIVERS_BUILD)/*.d $(DRIVERS_BUILD)/*.d.*
	rm -rf $(DRIVERS_BUILD)/*.lds

pre:
	mkdir -p $(DRIVERS_BUILD)
