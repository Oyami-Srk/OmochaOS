# GNO Makefile
# .

TOOLCHAIN_PREFIX 	= x86_64-elf-
KERNEL_OUT 			= $(BUILD)/kernel/kernel.out

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
UMOUNT  = umount

BXIMAGE = bximage
BOCHS 	= bochs

PY 		= python3

UNAME 	= $(shell uname)

# directories
SRC 		= $(shell pwd)/src
TOOLS 		= $(shell pwd)/tools
BUILD 		= $(shell pwd)/build
BUILD_TYPE 	= debug

export CC CXX LD OBJDUMP OBJCOPY ASM CP DD CAT SED PY SRC TOOLS BUILD BUILD_TYPE UNAME

.PHONY: all
all:
	mkdir -p $(BUILD)
	@echo "Building on $(UNAME) platform..."
	@echo "Built binary is under [$(BUILD)] with type like [$(BUILD_TYPE)]"
	@echo "So, ["$(CC)"] is used for compiling."
	make -C $(SRC)

.PHONY: boot
.PHONY: copy_img
ifneq ($(BUILD)/HD.img,$(wildcard $(BUILD)/HD.img))
copy_img:
	@echo "HD.img not found, copy from tools."
	$(CP) $(TOOLS)/HD_grub2.img $(BUILD)/HD.img
endif
ifneq ($(BUILD)/HD.img.lock,$(wildcard $(BUILD)/HD.img.lock))
boot: copy_img attach
else
boot:
endif
ifneq ($(KERNEL_OUT),$(wildcard $(KERNEL_OUT)))
	$(error Kernel not built yet)
endif
	test -n `find $(KERNEL_OUT) -a -size -65536c`
	$(CP) "$(KERNEL_OUT)" "$(shell $(CAT) $(BUILD)/HD.img.lock)/"
ifeq ($(TOOLS)/grub.cfg,$(wildcard $(TOOLS)/grub.cfg))
	$(CP) "$(TOOLS)/grub.cfg" "$(shell $(CAT) $(BUILD)/HD.img.lock)/boot/grub/"
endif
	$(CAT) $(TOOLS)/bochsrc_img_template | $(SED) -e 's/\".*\"/\"$(subst /,\/,$(BUILD))\/HD.img\"/g' > $(BUILD)/bochsrc

.PHONY: bochs
bochs: detatch
	$(BOCHS) -q -f $(BUILD)/bochsrc

.PHONY: attach
ifeq ($(UNAME), Darwin)
attach: BOOT_IMG_MOUNT_DIR ?= "$(shell hdiutil mount $(BUILD)/HD.img | grep '/Volumes/.*$$' -o)"
else ifeq ($(UNAME), Linux)
attach:
	$(error Boot disk creation process is not yet implemented on Linux. TODO: use losetup to map disk image on certain directorie)
endif
attach: attach_2

.PHONY: attach_2
attach_2:
	@echo Boot image is mounted to $(BOOT_IMG_MOUNT_DIR)
	@echo $(BOOT_IMG_MOUNT_DIR) > $(BUILD)/HD.img.lock

.PHONY: detatch
detatch:
ifneq ($(BUILD)/HD.img.lock,$(wildcard $(BUILD)/HD.img.lock))
	$(error Disk not mounted or lockfile is missing)
endif
	$(UMOUNT) "$(shell $(CAT) $(BUILD)/HD.img.lock)"
	$(RM) "$(BUILD)/HD.img.lock"
	
.PHONY: clean
clean:
	$(RM) -rf $(BUILD)/kernel
