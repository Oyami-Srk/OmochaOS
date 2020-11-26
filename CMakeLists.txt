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

enable_language(ASM_NASM) # need to setup first
enable_language(ASM)

set(CMAKE_C_FLAGS "-fno-pic -nostdinc -c -static -fno-builtin -fno-strict-aliasing -m32 -fno-stack-protector -g -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DACPI")

project(OmochaOS)
set(CMAKE_VERBOSE_MAKEFILE ON) # Set after project


file(GLOB_RECURSE CORE_C_SRCS "src/kernel/core/*.c")
file(GLOB_RECURSE CORE_ASM_SRCS "src/kernel/core/*.asm")
file(GLOB_RECURSE DRIVER_SRCS "src/kernel/driver/*.c")
file(GLOB_RECURSE LIB_SRCS "src/kernel/lib/*.c")
file(GLOB_RECURSE BUILTIN_MOD_SRCS "src/kernel/modules/*/*.c")
file(GLOB_RECURSE LIBSYS_SRCS src/kernel/lib/*.c)
file(GLOB_RECURSE INIT_C_SRCS "src/kernel/init/*.c")

set(TOOLS ${PROJECT_SOURCE_DIR}/tools)
message("Tools at " ${TOOLS})

set(SRCS ${CORE_C_SRCS} ${CORE_ASM_SRCS} ${DRIVER_SRCS} ${LIB_SRCS} ${BUILTIN_MOD_SRCS})
include_directories(src/kernel/header)

foreach (SRC in ITEMS ${CORE_ASM_SRCS})
    set_source_files_properties(${SRC} PROPERTIES LANGUAGE ASM_NASM)
endforeach ()

set(CFLAGS_LIST ${CMAKE_C_FLAGS})
string(REPLACE " " ";" CFLAGS_LIST "${CFLAGS_LIST}")

# Generated code
set(MULTIBOOT_I_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/multiboot.i)
set(MULTIBOOT_H ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/header/core/multiboot.h)
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
# End of Generated code

# Compile sys
add_library(sys STATIC ${LIBSYS_SRCS})
# set_target_properties(libos PROPERTIES PREFIX "") # remove prefix for compiled object
# End of Compile libos

# Compile init binary
add_executable(init.out ${INIT_C_SRCS})
target_link_libraries(init.out sys)
target_link_options(init.out PRIVATE -m elf_i386 -e init -Ttext 0x0)

add_library(init.bin OBJECT ${CMAKE_SOURCE_DIR}/src/kernel/init/package.S)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/kernel/init/package.S PROPERRIES LANGUAGE ASM)
target_compile_options(init.bin PRIVATE -m32)
target_compile_definitions(init.bin PRIVATE __BIN_PATH__="$(CMAKE_BINARY_DIR)/init.out")
add_dependencies(init.bin init.out)
# End of Compile init binary

# Compile kernel.elf
add_executable(kernel.elf ${SRCS})
add_dependencies(kernel.elf generate_precompiled)
target_link_libraries(kernel.elf init.bin)
set_target_properties(kernel.elf PROPERTIES LINK_FLAGS "-m elf_i386 -N -T${CMAKE_SOURCE_DIR}/src/kernel/kernel.ld ")
# End of Compile kernel.elf

# Generate HD.img
set(BUILD_IMAGE ${TOOLS}/build_image.sh)
set(BOOT_IMG boot.img)
add_custom_target(${BOOT_IMG}
        COMMAND ${BUILD_IMAGE} all ${CMAKE_BINARY_DIR}/${BOOT_IMG} ${CMAKE_BINARY_DIR}/kernel.elf ${TOOLS}/grub.cfg ${TOOLS}/root_dir/
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
add_dependencies(${BOOT_IMG} kernel.elf)
# End of Generate HD.img
