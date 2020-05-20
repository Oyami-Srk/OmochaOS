# GNU Makefile
# src

# max optimaze level is 2
CC_FLAGS = -fno-pic -nostdinc -c -static -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector -g -nostdlib 

CC_FLAGS += -DAPIC
CC_FLAGS += -DACPI
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
	$(MAKE) -C boot
	$(MAKE) -C kernel pre-build
	$(MAKE) -C kernel
	$(MAKE) -C modules pre-build
	$(MAKE) -C modules

