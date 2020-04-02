# GNU Makefile
# src/modules

MODULES_BUILD 	= $(BUILD)/modules
MODULES_LD_FLAGS = -m elf_i386 -N
MODULES_HEADER 	= $(HEADER)
MODS = $(foreach m,$(wildcard */),$(subst /,,$(m)))
MODS_OUT = $(foreach m,$(MODS),$(MODULES_BUILD)/$(m).out)
LIBOS = $(BUILD)/libos.a
LIBCRT0 = $(BUILD)/libcrt0.a

.PHONY: all
all: pre-build modules

.PHONY: create_dirs
create_dirs:
	@$(MKDIR) -p $(MODULES_BUILD)
	@$(MKDIR) -p $(foreach n, $(MODS), $(MODULES_BUILD)/$(n))

.PHONY: pre-build
pre-build: create_dirs

OBJS = $(foreach n, $(MODS), $(patsubst %, $(MODULES_BUILD)/$(MODS)/%.o, $(basename $(notdir $(wildcard $(n)/*.c)))))

$(MODULES_BUILD)/%.o: %.c
	$(CC) -o $@ -c $< $(CC_FLAGS) -I$(MODULES_HEADER)

$(MODULES_BUILD)/%.d: %.c
	@set -e; rm -f $@; $(CC) -MM $< -I$(MODULES_HEADER)  > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$

#$(MODULES_BUILD)/: $(OBJS)
#	$(LD) $(KERNEL_LD_FLAGS) -T kernel.ld -o $@ $(OBJS)

$(MODULES_BUILD)/%.out: $(MODULES_BUILD)/%/*.o
	$(LD) -m elf_i386 -Ttext 0x100000 -o $@ $^ $(LIBOS) $(LIBCRT0)
	$(CP) $@ $(ROOT_DIR)/$*.out

modules: $(LIBCRT0) $(OBJS) $(MODS_OUT)
	@echo Found modules: $(MODS)
	@echo Modules are built.

ifneq ($(filter pre-build,$(MAKECMDGOALS)),pre-build)
-include $(OBJS:.o=.d)
endif

$(LIBCRT0): libcrt0.asm
	$(ASM) -f elf -o $@ $^
