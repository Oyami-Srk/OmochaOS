TOOLS			= $(shell pwd)/tools


CC-FLAG-BOOT_STAGE1	= -fno-pic -O -nostdinc -c -static -MD -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector
CC-FLAG-BOOT_STAGE2 = -fno-pic -O -nostdinc -c -static -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector
BOOT_COMMON_HEADER	=	$(shell pwd)/common_header/

export TOOLS CC-FLAG-BOOT_STAGE1 CC-FLAG-BOOT_STAGE2 BOOT_COMMON_HEADER

.PHONY: all

all: binary img

binary:
	make -C stage1
	make -C stage2
	make -C stage2 clean

img:
	test -n `find $(BUILD)/boot/boot.bin -a -size -440c`
	test -n `find $(BUILD)/boot/loader.out -a -size -65536c`
ifneq ($(BUILD)/HD.img,$(wildcard $(BUILD)/HD.img))
	@echo "Create virtual flat image"
	$(BXIMAGE) -mode=create -hd=80M -q $(BUILD)/HD.img
	$(DD) if=$(TOOLS)/initialized_formated.bin of=$(BUILD)/HD.img seek=0 count=1 conv=notrunc
endif
	$(DD) if=$(BUILD)/boot/boot.bin count=1 bs=440 seek=0 conv=notrunc of=$(BUILD)/HD.img
	$(DD) if=$(BUILD)/boot/loader.out seek=1 conv=notrunc of=$(BUILD)/HD.img

boot:
	test -e $(BUILD)/HD.img
	$(CAT) $(TOOLS)/bochsrc_img_template | $(SED) -e 's/\".*\"/\"$(subst /,\/,$(BUILD))\/HD.img\"/g' > $(BUILD)/bochsrc
	$(BOCHS) -q -f $(BUILD)/bochsrc

.PHONY: clean
clean:
	rm -rf $(BUILD)/*
