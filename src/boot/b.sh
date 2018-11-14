nasm -f elf boot_sector.asm -I . -o bs_s.o
i386-elf-gcc -fno-pic -O -nostdinc -I. -c -static -MD -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector boot_sector.c -o bs_c.o
i386-elf-ld -N -e start -Ttext 0x7c00 -o bs.o bs_s.o bs_c.o
i386-elf-objdump -S bs.o > bs.asm
i386-elf-objcopy -S -O binary -j .text bs.o bs.bin
./sign.pl bs.bin
dd if=/dev/zero of=boot.img bs=512 count=2880
dd if=bs.bin of=boot.img count=1 seek=0 conv=notrunc
