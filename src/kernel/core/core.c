#include "core/memory.h"
#include "core/paging.h"
#include "driver/graphic.h"
#include "generic/typedefs.h"

void core_main(void *multiboot_header, void *magic) {
    GRAPHIC_write_color_string_to_vm(0, COLOR(BLACK, RED),
                                     "VenlafaxOS is booted.");
    while (1) {
        ;
    }
}

__attribute__((__aligned__(PG_SIZE))) unsigned int entry_page_dir[PDE_SIZE] = {
    [0]               = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
