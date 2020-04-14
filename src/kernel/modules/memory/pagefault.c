#include "buddy.h"
#include "core/paging.h"
#include "core/process.h"
#include "generic/asm.h"
#include "memory.h"
#include "paging.h"

#include "driver/graphic.h"

void do_page_fault(struct memory_info *mem, stack_frame *intf) {
    uint va = intf->trap_no;
    kprintf("Proc %d triggered PageFault with cr2 as %x.\n",
            ((process *)intf)->pid, va);

    magic_break();
    // when used then impl the rest, basicly code is fork.c:copy_parent:set
    // msg's page writable

    // test
    // map_pages(mem, ((process *)intf)->page_dir, (char *)va,
    //           page_alloc(mem, 1, PG_User | PG_Writeable | PG_Present),
    //           PG_SIZE, PG_User | PG_Writeable | PG_Present);

    // ((process *)intf)->status &= ~PROC_STATUS_ERROR;
}