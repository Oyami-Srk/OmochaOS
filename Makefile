# GNO Makefile
# .

UNAME 				= $(shell uname)
ifeq ($(UNAME), Darwin)
TOOLCHAIN_PREFIX 	= x86_64-elf-
else
TOOLCHAIN_PREFIX 	= 
endif
KERNEL_OUT 			= $(BUILD)/kernel/kernel.out
DEBUG_WAY 			= qemu

# Toolchain
CC 		= $(TOOLCHAIN_PREFIX)gcc
CXX 	= $(TOOLCHAIN_PREFIX)g++
LD 		= $(TOOLCHAIN_PREFIX)ld
AR 		= $(TOOLCHAIN_PREFIX)ar
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy

ASM 	= nasm

CP 		= cp
RSYNC 	= rsync
DD 		= dd
CAT 	= cat
SED 	= sed
AWK 	= awk
TEE 	= tee
RM 		= rm
MKDIR 	= mkdir
ifeq ($(UNAME), Darwin)
UMOUNT  = diskutil eject
else
UMOUNT 	= umount
endif

BXIMAGE = bximage
BOCHS 	= bochs
QEMU 	= qemu-system-i386

PY 		= python3
PY3 	= python3


# directories
SRC 		= $(shell pwd)/src
TOOLS 		= $(shell pwd)/tools
BUILD 		= $(shell pwd)/build
BUILD_TYPE 	= debug

BOOTIMG 		= $(BUILD)/HD.img
GRUB2IMG 		= $(TOOLS)/HD_grub2.img
GRUB2CFG 		= $(TOOLS)/grub.cfg
ROOT_DIR 		= $(TOOLS)/root_dir
QEMU_OPTIONS 	= -accel tcg,thread=single -m 128 -no-reboot -smp 1 -serial stdio -d cpu_reset,int,guest_errors -S -s -drive file=$(BOOTIMG)


export CC CXX LD AR OBJDUMP OBJCOPY ASM CP DD CAT SED AWK TEE RM MKDIR PY PY3 SRC TOOLS BUILD BUILD_TYPE ROOT_DIR UNAME

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
# kernel currently must smaller than 4MB
	test `wc -c <$(KERNEL_OUT)` -lt 4194304 
	$(CP) "$(KERNEL_OUT)" "$(shell $(CAT) $(BOOTIMG).lock)/"
ifeq ($(GRUB2CFG),$(wildcard $(GRUB2CFG)))
	$(CP) "$(GRUB2CFG)" "$(shell $(CAT) $(BOOTIMG).lock)/boot/grub/"
endif
	$(RSYNC) -avh --progress "$(ROOT_DIR)/" "$(shell $(CAT) $(BOOTIMG).lock)/"

.PHONY: bochs
bochs: detach
	$(CAT) $(TOOLS)/bochsrc_img_template | $(SED) -e 's/\".*\"/\"$(subst /,\/,$(BOOTIMG))\"/g' > $(BUILD)/bochsrc
	-$(BOCHS) -q -f $(BUILD)/bochsrc
	@echo "Simulation Terminated"

.PHONY: qemu
qemu: detach
	-$(QEMU) $(QEMU_OPTIONS)
	@echo "Simulation Terminated"

.PHONY: attach
ifeq ($(UNAME), Darwin)
attach: BOOT_IMG_MOUNT_DIR ?= "$(shell hdiutil mount $(BOOTIMG) | grep '/Volumes/.*$$' -o)"
else ifeq ($(UNAME), Linux)
attach:
endif
attach: attach_2

.PHONY: attach_2
attach_2:
ifeq ($(UNAME), Linux)
ifneq ($(BOOTIMG).lock,$(wildcard $(BOOTIMG).lock))
	$(eval loop_device := $(shell udisksctl loop-setup -f $(BOOTIMG) | grep -o -e '/dev/loop[0-9]*'))
	@echo Boot image is setup to loop device $(loop_device)
	@echo $(loop_device) > $(BOOTIMG).lock.loop
	$(eval BOOT_IMG_MOUNT_DIR ?= $(shell udisksctl mount -b $(loop_device)p1 | grep -o -e '/media/.*[0-9A-Za-z]'))
else
	@echo "Image is already mounted."
	$(eval BOOT_IMG_MOUNT_DIR ?= $(shell cat $(BOOTIMG).lock))
endif
endif
	@echo Boot image is mounted to $(BOOT_IMG_MOUNT_DIR)
	@echo $(BOOT_IMG_MOUNT_DIR) > $(BOOTIMG).lock

.PHONY: detach
detach:
ifneq ($(BOOTIMG).lock,$(wildcard $(BOOTIMG).lock))
	@echo Disk not mounted or lockfile is missing
else
	$(UMOUNT) "$(shell $(CAT) $(BOOTIMG).lock)"
	$(RM) "$(BOOTIMG).lock"
ifeq ($(UNAME), Linux)
	udisksctl loop-delete -b $(shell $(CAT) $(BOOTIMG).lock.loop)
	$(RM) "$(BOOTIMG).lock.loop"
endif
endif

.PHONY: debug
debug:
	$(MAKE)
	$(MAKE) disasm
	$(MAKE) boot
	$(MAKE) $(DEBUG_WAY)
	
.PHONY: clean
clean:
	$(RM) -rf $(BUILD)

.PHONY: disasm
disasm:
	$(OBJDUMP) -S $(KERNEL_OUT) > $(BUILD)/kernel.disasm
