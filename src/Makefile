# GNU Makefile
# src

# max optimaze level is 2
CC_FLAGS = -fno-pic -nostdinc -c -static -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector -g
HEADER = $(shell pwd)/header

ifeq ($(BUILD_TYPE), debug)
	CC_FLAGS += -D__DEBUG__
endif

export CC_FLAGS HEADER


.PHONY: all
all:
ifeq ($(and $(CC),$(CXX),$(LD),$(OBJDUMP),$(OBJCOPY),$(ASM),$(DD),$(CAT),$(SED),$(BUILD),$(BUILD_TYPE)),)
	$(error Build cannot progress, because toolchain is not set)
endif
	@echo Build with CCFLAGS "$(CC_FLAGS)"
	make -C boot
	make -C kernel pre-build
	make -C kernel
	make -C modules pre-build
	make -C modules

