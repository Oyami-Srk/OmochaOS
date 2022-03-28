# OmochaOS
OmochaOS is my personal studying project. Some code and lower design is inherited from my old project archive branch [OmochaOS(original archive)](https://github.com/Oyami-Srk/OmochaOS/tree/original_omochaos). Why am I interseted in OS-Developing is under the readme of that branch.
This version of OmochaOS will be rewrite from scratch.


# Build
This Project is cmake-based, simply built the target `kernel.elf` for the `multiboot` standard kernel executable.

* #### dependencies
    * python3 (with pyyaml supported)
    * compiling-toolchain (gcc ar ld nm objdump objcopy)
        * set `TOOLCHAIN_PREFIX` in Makefile to use croee-compiler, this project is developed under MacOS and built passed on Linux-based system. If you are using MacOS, install `x86_64-elf-binutils` and `x86_64-elf-gcc` is good enough. If you are using Linux, just make sure above tools is installed. Otherwise you may figure it out on your platform.
    * nasm
    * useful utilities (cat sed awk tee)
* #### build process
    * compile kernel's core and modules within kernel space into object
    * link some general used code and syscall wrapping into `libos.a`
    * compile init code (linked with `libos.a`) into elf executable file `init.out`
    * compile init `package.S` to object `init.out.o` which contains full content of `init.out` by using `.incbin`
    * link kernel object and `init.out.o` to `kernel.out`
    * build modules (not modules in kernel dir that's builtin modules)

# Build boot image
Build the cmake target `boot.img` for bootable raw disk image.
This command is depend on `tools/boot_image` which only implement on MacOS (based on `hdiutil` and `diskutil` comes with macos) and Linux (based on `udisksctl` controlled loop-device). If you cannot make it, try to implement it yourself or build image yourself.
This command uses `rsync` to copy everything in `tools/root_dir` to image's root dir and `cp` to copy `grub.cfg` to `/boot/grub/grub.cfg` of image.
This command copies `tools/HD_grub2.img` (which contains a formatted disk image with grub2 installed and a FAT32 part. FAT32 part is the first part of disk, contains grub2 boot files. You may probably generate it yourself via `dd`)

# Run
Build the cmake target `RunQemu` for running bootable image on qemu. (or `Bochs`)
* #### dependencies
    * bochs or qemu

# Debug
Build the cmake target `DebugQemu` for QEMU with GDB-Remote Debug functionality.

# System Design
Omochaos is micro-kernel but with *memory* and *systask* running in kernel space (has kernel symbol linked)
Currently most of modules are running in kernel space. Those modules will exit when modules from disk is properly loaded.
All of modules are running under ring 1. Only two syscall `__recv_msg` and `__send_msg`, `interrupt_handler` are running under ring 0 after core finish boot up.
`systask` should give a common way to access core data in `struct environment`(not impl yet)

# Road Map
* VFS
* Port Newlib
* Shabby Shell
* ACPI
* SMP Support
* Process scheduler
* Graphic Interface

# Thanks
[![CLion](./resource/icon_CLion.png)](https://www.jetbrains.com/?from=OmochaOS)
This Project is development under CLion with my student license.