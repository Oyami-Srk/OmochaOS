# GNU Makefile
# src/boot/stage2

.PHONY: all

BOOT_BUILD	= $(BUILD)/boot

SRCS := $(wildcard *.c)
OBJS := $(pathsubst %, $(BOOT_BUILD)/%.o, $(basename $(SRCS)))

all: $(BOOT_BUILD)/loader.out

$(BOOT_BUILD)/loader.out: $(BOOT_BUILD)/loader.o $(OBJS)
	$(LD) -o $@ $^

$(BOOT_BUILD)/%.o: %.c
	$(CC) -o $@ -c $< $(CC-FLAG-BOOT_STAGE2) -I$(BOOT_COMMON_HEADER)

$(BOOT_BUILD)/%.d: %.c
	@set -e; rm -f $@; $(CC) -MM $< -I$(BOOT_COMMON_HEADER)  > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
  rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -f $(BOOT_BUILD)/*.d $(BOOT_BUILD)/*.d.*
