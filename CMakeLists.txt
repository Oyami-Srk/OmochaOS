cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86)

if (APPLE)
    message("Setting corss-compiler for macos")
    # set cross-compiler for macos
    set(CMAKE_CROSSCOMPILING TRUE)
    set(CMAKE_C_COMPILER x86_64-elf-gcc)
    set(CMAKE_CXX_COMPILER x86_64-elf-g++)
    set(CMAKE_AR x86_64-elf-ar)
    set(CMAKE_RANLIB x86_64-elf-ranlib)
    set(CMAKE_C_LINK_EXECUTABLE "x86_64-elf-ld <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
    set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf")

    set(CMAKE_C_COMPILER_WORKS TRUE) # Pass Cmake Compiler tests manually
    set(CMAKE_CXX_COMPILER_WORKS TRUE)
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # Remove annoying -rdynamic option
endif (APPLE)

if(UNIX AND NOT APPLE)
    find_program(LD_LINKER ld)
    set(CMAKE_C_LINK_EXECUTABLE "${LD_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
    find_program(CMAKE_C_COMPILER_NM nm)
    find_program(CMAKE_C_OBJDUMP objdump)
endif(UNIX AND NOT APPLE)


enable_language(ASM_NASM) # need to setup first
enable_language(ASM)

set(CMAKE_C_FLAGS "-fno-pic -nostdinc -static -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector -g -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DACPI")

project(OmochaOS)
set(CMAKE_VERBOSE_MAKEFILE ON) # Set after project

find_program(AWK awk mawk gawk)
find_program(SED sed)
find_program(GREP grep ggrep)
find_program(SORT sort)
find_program(CAT cat)
find_program(BOCHS bochs)


file(GLOB_RECURSE ASM_SRCS "src/kernel/*/*.S")
file(GLOB_RECURSE C_SRCS "src/kernel/*/*.c")

set(TOOLS ${PROJECT_SOURCE_DIR}/tools)
message("Tools at " ${TOOLS})

set(SRCS ${ASM_SRCS} ${C_SRCS})
include_directories(src/kernel/headers)

foreach (SRC in ITEMS ${ASM_SRCS})
    set_source_files_properties(${SRC} PROPERTIES COMPILE_DEFINITIONS ASM_FILE)
    set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS -m32)
endforeach ()

set(CFLAGS_LIST ${CMAKE_C_FLAGS})
string(REPLACE " " ";" CFLAGS_LIST "${CFLAGS_LIST}")

# Compile kernel.elf
add_executable(kernel.elf ${SRCS})
set_target_properties(kernel.elf PROPERTIES LINK_FLAGS "-m elf_i386 -N -T${CMAKE_SOURCE_DIR}/src/kernel/kernel.ld")
# End of Compile kernel.elf

# Generate Symbol File
add_custom_target(kernel.bsb
        COMMAND ${CMAKE_C_COMPILER_NM} kernel.elf | ${SORT} | ${AWK} '{ print $$1\" \"$$3 }' > kernel.bsb
        DEPENDS kernel.elf
)
# End of Generate Symbol File

# Generate Disassembly Code
add_custom_target(kernel.disasm
        COMMAND ${CMAKE_C_OBJDUMP} -S kernel.elf > kernel.disasm
        DEPENDS kernel.elf
)
# End of Generate Disassembly Code

# Generate HD.img
set(BUILD_IMAGE ${TOOLS}/build_image.sh)
set(BOOT_IMG boot.img)
add_custom_target(${BOOT_IMG}
        COMMAND ${BUILD_IMAGE} all ${CMAKE_BINARY_DIR}/${BOOT_IMG} ${CMAKE_BINARY_DIR}/kernel.elf ${TOOLS}/grub.cfg ${TOOLS}/root_dir/
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
add_dependencies(${BOOT_IMG} kernel.elf)
# End of Generate HD.img

# Running Qemu
set(BOOT_SCRIPT ${TOOLS}/boot_image.sh)
set(BOOT_IMG boot.img)
add_custom_target(RunQEMU
        COMMAND ${BOOT_SCRIPT} qemu run ${CMAKE_BINARY_DIR}/${BOOT_IMG}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        DEPENDS ${BOOT_IMG}
        )
# End of Running Qemu

# Debuging Qemu
set(BOOT_IMG boot.img)
add_custom_target(DebugQEMU
        COMMAND ${BOOT_SCRIPT} qemu debug ${CMAKE_BINARY_DIR}/${BOOT_IMG}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        DEPENDS ${BOOT_IMG}
        )
# End of Debuging Qemu

# Running Bochs
string(REPLACE "/" "\\/" SED_USED_BOOT_IMG "${CMAKE_BINARY_DIR}/${BOOT_IMG}")
string(REPLACE "/" "\\/" SED_USED_KERNEL_BSB "${CMAKE_BINARY_DIR}/kernel.bsb")

add_custom_target(bochsrc
        COMMAND sh -c \"${CAT} ${TOOLS}/bochsrc_img_template |
                        ${SED} -e 's/\\"80m.img\\"/\\"${SED_USED_BOOT_IMG}\\"/g' |
                        ${SED} -e 's/\\"dbg.bsb\\"/\\"${SED_USED_KERNEL_BSB}\\"/g'
                        > bochsrc\"
        DEPENDS kernel.bsb
        DEPENDS ${BOOT_IMG}
)

add_custom_target(Bochs
        COMMAND ${BOCHS} -q -f bochsrc 2>&1
        DEPENDS bochsrc
)
# End of Running Bochs