# OmochaOS
OmochaOS is my personal studying project. Some code and lower design is inherited from my old project archive branch [OmochaOS(original archive)](https://github.com/Oyami-Srk/OmochaOS/tree/original_omochaos). Why am I interseted in OS-Developing is under the readme of that branch.
> This branch of OmochaOS is directly forked from VenlafaxOS which was already archived.
> I will face my future and break the dependence of medicine. So I deciced not to use VenlafaxOS as a name anymore.

#### Now waiting for the exam score to come out at 7/26 5 PM

# Build
Run `make` in project root dir.
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
Run `make boot` in project root dir.
This command is depend on `make attach` and `make detach` which only implement on MacOS (based on `hdiutil` and `diskutil` comes with macos) and Linux (based on `udisksctl` controlled loop-device). If you cannot make it, try to implement it yourself or build image yourself.
This command uses `rsync` to copy everything in `tools/root_dir` to image's root dir and `cp` to copy `grub.cfg` to `/boot/grub/grub.cfg` of image.
This command copies `tools/HD_grub2.img` (which contains a formatted disk image with grub2 installed and a FAT32 part. FAT32 part is the first part of disk, contains grub2 boot files.)

# Run
Run `make qemu` or `make bochs` in project root dir.
* #### dependencies
    * bochs or qemu

# Debug
If qemu/bochs is installed and `make attach/detach` is implemented. You can use `make debug DEBUG_WAY=qemu/bochs` to build and run in one command.

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
