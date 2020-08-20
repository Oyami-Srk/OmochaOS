#include <buddy.h>
#include <core/memory.h>
#include <core/paging.h>
#include <core/process.h>
#include <core/protect.h>
#include <generic/asm.h>
#include <lib/elf.h>
#include <lib/stdlib.h>
#include <memory.h>
#include <paging.h>

#include <modules/tty.h>

BOOL elf_check_file(Elf32_Ehdr *hdr); // execve.c
BOOL elf_check_supported(Elf32_Ehdr *hdr);

static void load_elf_to_proc(struct memory_info *mem, process *proc,
                             ubyte *data) {
    Elf32_Ehdr *elf_header = (Elf32_Ehdr *)data;
    BOOL        is_valid   = elf_check_supported(elf_header);
    if (!is_valid) {
        panic("INIT CODE IS NOT PROPER ELF FORMAT");
        printf("[MEM] INIT CODE IS NOT ELF FORMAT");
        magic_break();
    }
    struct prog_info *pgi = proc->prog_info =
        (struct prog_info *)mem_kmalloc(sizeof(struct prog_info));

    proc->prog_info->image_start   = (char *)0xFFFFFFFF;
    proc->prog_info->program_break = 0;
    proc->prog_info->program_size  = 0;

    // load prog header
    assert(sizeof(Elf32_Phdr) == elf_header->e_phentsize);
    assert(sizeof(Elf32_Shdr) == elf_header->e_shentsize);
    Elf32_Phdr *prog_header = (Elf32_Phdr *)(data + elf_header->e_phoff);
    for (uint i = 0; i < elf_header->e_phnum; i++) {
        if (prog_header[i].p_type == PT_LOAD) {
            if ((uint)pgi->image_start > prog_header[i].p_vaddr) {
                pgi->image_start = (char *)prog_header[i].p_vaddr;
            }
            if ((uint)pgi->program_break <
                prog_header[i].p_vaddr + prog_header[i].p_memsz) {
                pgi->program_break =
                    (char *)(prog_header[i].p_vaddr + prog_header[i].p_memsz);
            }
            size_t memsz = prog_header[i].p_memsz;
            memsz        = PGROUNDUP(memsz);

            char *pa        = page_alloc(mem, memsz / PG_SIZE,
                                  PAGE_TYPE_INUSE | PAGE_TYPE_USER);
            char *va        = (char *)prog_header[i].p_vaddr;
            uint  va_offset = (uint)va - PGROUNDDOWN((uint)va);

            memcpy(pa + va_offset, data + prog_header[i].p_offset,
                   prog_header[i].p_filesz);

            if (va_offset)
                memset(pa, 0, va_offset);
            if (prog_header[i].p_filesz != prog_header[i].p_memsz) {
                if (prog_header[i].p_filesz > prog_header[i].p_memsz) {
                    printf("[MEM] Prog header filesz bigger than memsize");
                    magic_break();
                }
                memset(pa + va_offset + prog_header[i].p_filesz, 0,
                       prog_header[i].p_memsz - prog_header[i].p_filesz);
            }
            map_pages(
                mem, proc->page_dir, (void *)PGROUNDDOWN((uint)va), pa,
                prog_header[i].p_memsz,
                PG_Present | PG_User |
                    ((prog_header[i].p_flags & PF_W) != 0 ? PG_Writable : 0));
        }
    }

    pgi->program_size = pgi->program_break - pgi->image_start;
    proc->stack.eip   = elf_header->e_entry;
}

void start_up_init(struct memory_info *mem, ubyte *init_elf) {
    char *   pstack_top    = (char *)KERN_BASE;
    char *   pstack_bottom = (char *)(KERN_BASE - PROC_STACK_SIZE);
    process *init_proc     = get_proc(1);
    init_proc->status      = PROC_STATUS_STOP;

    init_proc->stack.cs = SEL_CODE_DPL3;
    init_proc->stack.ds = SEL_DATA_DPL3;
    init_proc->stack.es = SEL_DATA_DPL3;
    init_proc->stack.fs = SEL_DATA_DPL3;
    init_proc->stack.gs = SEL_DATA_DPL3;
    init_proc->stack.ss = SEL_DATA_DPL3;

    init_proc->pstack       = pstack_bottom;
    char *stack             = page_alloc(mem, PROC_STACK_SIZE / PG_SIZE,
                             PAGE_TYPE_INUSE | PAGE_TYPE_USER); // 8KB
    init_proc->pstack_size  = PROC_STACK_SIZE;
    init_proc->stack.esp    = (uint)pstack_top;
    init_proc->stack.eflags = 0x1202;
    init_proc->page_dir     = create_page_dir(mem, PG_Writable);
    memcpy(init_proc->name, "init", 5);
    map_pages(mem, (pde_t *)init_proc->page_dir, init_proc->pstack, stack,
              init_proc->pstack_size, PG_Present | PG_Writable | PG_User);

    printf("[MEM] Loading init.out\n");
    load_elf_to_proc(mem, init_proc, init_elf);

    init_proc->pid        = 1;
    init_proc->parent_pid = 0;

    extern process **proc_list;  // process.c
    extern size_t *  proc_count; // process.c

    process *p = *proc_list;
    process *f = p;
    while (p) {
        f = p;
        p = p->next;
    }
    init_proc->next = NULL;
    f->next         = init_proc;
    (*proc_count)++;
    init_proc->status = PROC_STATUS_RUNNING | PROC_STATUS_NORMAL;
}

// page_id = (page_paddr - pstart) / PG_SIZE
// pg_count = (end-start) / PG_SIZE
// buddy bits = (pg_count / (2^order)) / 2 = page_count >> (order+1)
// total buddy bits = ((2^MAX_ORDER)-1)*pg_count/(2^MAX_ORDER)

void init_memory(struct memory_info *mem) {
    printf("[MEM] Init memory 0x%x - 0x%x\n", mem->memory_start,
           mem->memory_end);
    size_t pg_count     = (mem->memory_end - mem->memory_start) / PG_SIZE;
    size_t pg_info_size = pg_count * sizeof(struct page);
    mem->page_count     = pg_count;
    uint buddy_total_bits =
        ((1 << (MAX_ORDER)) - 1) * pg_count / (1 << MAX_ORDER);
    size_t buddy_bytes    = buddy_total_bits / 8;
    uint   max_block_size = (1 << (MAX_ORDER - 1)) * PG_SIZE;
    mem->usable_end       = ROUNDDOWN_WITH(
        max_block_size, (mem->memory_end - buddy_bytes - pg_info_size));
    mem->pages_info    = (struct page *)(mem->usable_end);
    mem->buddy_map[0]  = (bitset *)(mem->memory_end - buddy_bytes);
    mem->free_list[0]  = (block_list *)NULL;
    mem->free_count[0] = 0;
    printf("[MEM] Init buddy map on 0x%x, size %d bytes.\n", mem->buddy_map,
           buddy_bytes);
    printf("[MEM] Init pages info on %x, size %d bytes.\n", mem->pages_info,
           pg_info_size);
    printf("[MEM] Total occupied mem size: %d bytes(%d megabytes).\n",
           mem->memory_end - mem->usable_end,
           (mem->memory_end - mem->usable_end) / 1024 / 1024);
    memset((void *)mem->usable_end, 0, mem->memory_end - mem->usable_end);
    for (uint i = 0; i < MAX_ORDER - 1; i++) {
        mem->buddy_map[i + 1] =
            (bitset *)(((uint)mem->buddy_map[i]) + (pg_count >> (i + 1)) / 8);
        mem->free_list[i + 1]  = (block_list *)NULL;
        mem->free_count[i + 1] = 0;
    }
    void *pg = NULL;
    for (pg = (void *)mem->memory_start; pg < (void *)mem->usable_end;
         pg += max_block_size) {
        // free max block first
        block_list *current = (block_list *)pg;
        // attach_to_free_list(mem, current, MAX_ORDER - 1);

        current->next                 = mem->free_list[MAX_ORDER - 1];
        current->next->prev           = current;
        current->prev                 = NULL;
        mem->free_list[MAX_ORDER - 1] = current;

        mem->free_count[MAX_ORDER - 1]++;
    }
    printf("[MEM] pages info size: %d KB\n",
           pg_count * sizeof(struct page) / 1024);
    for (uint i = 0; i < pg_count; i++) {
        if ((char *)GET_PAGE_BY_ID(mem, i) < (char *)mem->usable_end)
            mem->pages_info[i].type = PAGE_TYPE_FREE | PAGE_TYPE_USABLE;
        else
            mem->pages_info[i].type = PAGE_TYPE_INUSE | PAGE_TYPE_SYSTEM;
    }
    printf("[MEM] Initialized. Total %d pages. 4M Block Count: %d.\n", pg_count,
           mem->free_count[MAX_ORDER - 1]);
    return;
}