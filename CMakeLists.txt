# CMake is only for project generate
# Please use Makefile to compile

cmake_minimum_required(VERSION 3.0.0)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_CROSSCOMPILING true)
set(CAN_USE_ASSEMBLER TRUE)
if (APPLE)
    message(Setting corss-compiler for macos)
    # set cross-compiler for macos
    SET(CMAKE_C_COMPILER x86_64-elf-gcc)
    SET(CMAKE_CXX_COMPILER x86_64-elf-g++)
    SET(CMAKE_C_LINK_EXECUTABLE x86_64-elf-ld)
    SET(CMAKE_ASM_NASM_OBJECT_FORMAT "elf")
endif (APPLE)

project(VenlafaxOS)

set(CMAKE_C_FLAGS "-fno-pic -nostdinc -c -static -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector -g -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DACPI")
enable_language(ASM_NASM)
SET(CMAKE_EXE_LINKER_FLAGS "-m elf_i386 -N -T ${CMAKE_SOURCE_DIR}/src/kernel/kernel.ld")
# set(CMAKE_ASM_NASM_LINK_EXECUTABLE "ld <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_ASM_NASM_LINK_EXECUTABLE "ld <FLAGS> <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")


FILE(GLOB_RECURSE CORE_C_SRCS "src/kernel/core/*.c")
FILE(GLOB_RECURSE CORE_ASM_SRCS "src/kernel/core/*.asm")
FILE(GLOB_RECURSE DRIVER_SRCS "src/kernel/driver/*.c")
FILE(GLOB_RECURSE LIB_SRCS "src/kernel/lib/*.c")
FILE(GLOB_RECURSE BUILTIN_MOD_SRCS "src/kernel/modules/*/*.c")

FILE(GLOB_RECURSE LIBOS_SRCS src/kernel/lib/*.c)

FILE(GLOB_RECURSE INIT_C_SRCS "src/kernel/init/*.c")

set(SRCS ${CORE_C_SRCS} ${CORE_ASM_SRCS} ${DRIVER_SRCS} ${LIB_SRCS} ${BUILTIN_MOD_SRCS})
message(${SRCS})
include_directories(src/kernel/header)

foreach (SRC in ITEMS ${CORE_ASM_SRCS})
    set_source_files_properties(${SRC} PROPERTIES LANGUANE ASM_NASM)
endforeach ()

set(MULTIBOOT_I_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/multiboot.i)
set(MULTIBOOT_H ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/header/core/multiboot.h)
message(${MULTIBOOT_H})
message(${MULTIBOOT_I_OUTPUT})

set(CFLAGS_LIST ${CMAKE_C_FLAGS})
string(REPLACE " " ";" CFLAGS_LIST "${CFLAGS_LIST}")

add_custom_command(
        OUTPUT ${MULTIBOOT_I_OUTPUT}
        DEPENDS src/kernel/header/core/multiboot.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND gcc ${CFLAGS_LIST} -E -dM ${MULTIBOOT_H} |
        awk -F' ' 'substr\($$2,0,1\)!="_" {gsub\(/\#/,"%"\)\; print}'
        > ${MULTIBOOT_I_OUTPUT}
        COMMENT "Generates the nasm multiboot header file."
)

add_custom_target(generate_precompiled DEPENDS
        "${MULTIBOOT_I_OUTPUT}")

add_library(
        kernel STATIC
        ${SRCS}
)

add_dependencies(kernel generate_precompiled init)
add_custom_command(
        TARGET kernel
        POST_BUILD
        COMMAND ${CMAKE_C_LINK_EXECUTABLE} -m elf_i386 -N -T${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/kernel.ld ${CMAKE_BINARY_DIR}/libkernel.a ${CMAKE_BINARY_DIR}/init.out.o -o kernel.elf
)

add_library(
        libos STATIC
        ${LIBOS_SRCS}
)
set_target_properties(libos PROPERTIES PREFIX "")

add_library(
        init STATIC
        ${INIT_C_SRCS}
)

add_dependencies(init libos)

add_custom_command(
        TARGET init
        POST_BUILD
        COMMAND ${CMAKE_C_LINK_EXECUTABLE} -m elf_i386 -e init -Ttext 0x0 -o init.out ${CMAKE_BINARY_DIR}/libinit.a ${CMAKE_BINARY_DIR}/libos.a
        COMMAND ${CMAKE_C_COMPILER} -m32 -D__BIN_PATH__='"$(CMAKE_BINARY_DIR)/init.out"' ${CMAKE_SOURCE_DIR}/src/kernel/init/package.S -c -o init.out.o
)

