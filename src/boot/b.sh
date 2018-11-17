nasm -f elf boot_sector.asm -I . -o bs_s.o
i386-elf-gcc -fno-pic -O -nostdinc -I. -c -static -MD -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector boot_sector.c -o bs_c.o
i386-elf-ld -N -e start -Ttext 0x7c00 -o bs.o bs_s.o bs_c.o
i386-elf-objdump -S bs.o > bs.disasm
i386-elf-objcopy -S -O binary -j .text bs.o bs.bin
./sign.pl bs.bin

rm 80m.img
bximage -mode=create -hd=80M -q 80m.img
dd if=bs.bin of=80m.img count=1 seek=0 conv=notrunc
