# GNO Makefile
# .

UNAME 				= $(shell uname)
TOOLCHAIN_PREFIX 	= x86_64-elf-
KERNEL_OUT 			= $(BUILD)/kernel/kernel.out
DEBUG_WAY 			= qemu

# Toolchain
CC 		= $(TOOLCHAIN_PREFIX)gcc
CXX 	= $(TOOLCHAIN_PREFIX)g++
LD 		= $(TOOLCHAIN_PREFIX)ld
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy

ASM 	= nasm

CP 		= cp
DD 		= dd
CAT 	= cat
SED 	= sed
AWK 	= awk
ifeq ($(UNAME), Darwin)
UMOUNT  = diskutil eject
else
UMOUNT 	= eject
endif

BXIMAGE = bximage
BOCHS 	= bochs
QEMU 	= qemu-system-i386

PY 		= python3


# directories
SRC 		= $(shell pwd)/src
TOOLS 		= $(shell pwd)/tools
BUILD 		= $(shell pwd)/build
BUILD_TYPE 	= debug

BOOTIMG 		= $(BUILD)/HD.img
GRUB2IMG 		= $(TOOLS)/HD_grub2.img
GRUB2CFG 		= $(TOOLS)/grub.cfg
QEMU_OPTIONS 	= -accel tcg,thread=single -m 128 -no-reboot -smp 1 -serial stdio -d cpu_reset,int,guest_errors -S -s -drive file=$(BOOTIMG)


export CC CXX LD OBJDUMP OBJCOPY ASM CP DD CAT SED AWK PY SRC TOOLS BUILD BUILD_TYPE UNAME

.PHONY: all
all:
	mkdir -p $(BUILD)
	@echo "Building on $(UNAME) platform..."
	@echo "Built binary is under [$(BUILD)] with type like [$(BUILD_TYPE)]"
	@echo "So, ["$(CC)"] is used for compiling."
	make -C $(SRC)

.PHONY: boot
.PHONY: copy_img
ifneq ($(BOOTIMG),$(wildcard $(BOOTIMG)))
copy_img:
	@echo "$(BOOTIMG) not found, copy from tools."
	$(CP) $(GRUB2IMG) $(BOOTIMG)
endif
ifneq ($(BOOTIMG).lock,$(wildcard $(BOOTIMG).lock))
boot: copy_img attach
else
boot:
endif
ifneq ($(KERNEL_OUT),$(wildcard $(KERNEL_OUT)))
	$(error Kernel not built yet)
endif
	test -n `find $(KERNEL_OUT) -a -size -65536c`
	$(CP) "$(KERNEL_OUT)" "$(shell $(CAT) $(BOOTIMG).lock)/"
ifeq ($(GRUB2CFG),$(wildcard $(GRUB2CFG)))
	$(CP) "$(GRUB2CFG)" "$(shell $(CAT) $(BOOTIMG).lock)/boot/grub/"
endif

.PHONY: bochs
bochs: detatch
	$(CAT) $(TOOLS)/bochsrc_img_template | $(SED) -e 's/\".*\"/\"$(subst /,\/,$(BOOTIMG))\"/g' > $(BUILD)/bochsrc
	$(BOCHS) -q -f $(BUILD)/bochsrc

.PHONY: qemu
qemu: detatch
	$(QEMU) $(QEMU_OPTIONS)

.PHONY: attach
ifeq ($(UNAME), Darwin)
attach: BOOT_IMG_MOUNT_DIR ?= "$(shell hdiutil mount $(BOOTIMG) | grep '/Volumes/.*$$' -o)"
else ifeq ($(UNAME), Linux)
attach:
	$(error Boot disk creation process is not yet implemented on Linux. TODO: use losetup to map disk image on certain directorie)
endif
attach: attach_2

.PHONY: attach_2
attach_2:
	@echo Boot image is mounted to $(BOOT_IMG_MOUNT_DIR)
	@echo $(BOOT_IMG_MOUNT_DIR) > $(BOOTIMG).lock

.PHONY: detatch
detatch:
ifneq ($(BOOTIMG).lock,$(wildcard $(BOOTIMG).lock))
	@echo Disk not mounted or lockfile is missing
else
	$(UMOUNT) "$(shell $(CAT) $(BOOTIMG).lock)"
	$(RM) "$(BOOTIMG).lock"
endif

.PHONY: debug
debug: 
	$(MAKE)
	$(MAKE) boot
	$(MAKE) $(DEBUG_WAY)
	
.PHONY: clean
clean:
	$(RM) -rf $(BUILD)/kernel
