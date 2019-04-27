# GNU Makefile
# src/

HEADER = $(shell pwd)/header
CC_FLAGS = -fno-pic -O -nostdinc -c -static -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector -g

export HEADER CC_FLAGS

.PHONY: all
all:
	make -C boot
	make -C lib
	make -C drivers
	make -C modules
	make -C kernel
