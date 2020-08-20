#include <buddy.h>
#include <core/memory.h>
#include <core/paging.h>
#include <core/process.h>
#include <generic/asm.h>
#include <generic/typedefs.h>
#include <memory.h>
#include <paging.h>

#include <driver/graphic.h>

void do_page_fault(struct memory_info *mem, stack_frame *intf) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
    process *caller = (process *)intf;
#pragma GCC diagnostic pop

    void *va = (void *)intf->trap_no;
    void *pa = (void *)vir2phy(caller->page_dir, va);
    // kprintf("Proc %d triggered PageFault with cr2 as %x.\n", caller->pid,
    // va);

    int ref = get_page_reference(mem, (void *)PGROUNDDOWN((uint)pa));
    // kprintf("ref: %d\n", ref);
    if (ref == 1) {
        // only caller got the reference
        *get_pte(caller->page_dir, va) |= PG_Writable;
        caller->status &= ~PROC_STATUS_ERROR;
    } else if (ref > 1) {
        // both caller and other got the reference
        void *old_pa = (void *)PGROUNDDOWN((uint)pa);
        void *new_pa = page_alloc(mem, 1, PAGE_TYPE_INUSE | PAGE_TYPE_USER);
        memcpy(new_pa, old_pa, PG_SIZE);
        decrease_page_ref(mem, old_pa);

        pde_t *pde    = &caller->page_dir[(uint)va >> 22];
        pte_t *pg_tab = (pte_t *)(*pde & ~0xFFF);
        int    pg_ref = get_page_reference(mem, (void *)pg_tab);
        if (pg_ref == 1) {
            // pg_tab is only for caller
            // no need change
        } else if (pg_ref > 1) {
            // pg_tab is shared with others
            // alloc one and copy
            decrease_page_ref(mem, pg_tab);
            pte_t *new_pg_tab =
                (pte_t *)page_alloc(mem, 1, PAGE_TYPE_INUSE | PAGE_TYPE_PGTBL);
            memcpy(new_pg_tab, pg_tab, PG_SIZE);
            *pde = (uint)new_pg_tab | PG_Present | PG_User | PG_Writable;
        }
        *get_pte(caller->page_dir, va) =
            (uint)new_pa | PG_Present | PG_User | PG_Writable;
        caller->status &= ~PROC_STATUS_ERROR;
    } else {
        panic("wrong page references number.");
        magic_break();
    }
}