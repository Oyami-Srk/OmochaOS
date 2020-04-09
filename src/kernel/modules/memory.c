/*
module:
  name: Memory
  author: Shiroko
  summary: Memory Process
  entry: Task_Memory
*/
#include "core/memory.h"
#include "core/environment.h"
#include "core/paging.h"
#include "generic/asm.h"
#include "lib/bitset.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "modules/tty.h"

unsigned int sys_mapping[][3] = {
    // paddr, vaddr, page size
    /* {0, 0, PG_SIZE * 1024}, */
    {0, KERN_BASE, PG_SIZE * 1024}};

#define PAGE_TYPE_USABLE   0x01
#define PAGE_TYPE_RESERVED 0x02
#define PAGE_TYPE_SYSTEM   0x04
#define PAGE_TYPE_HARDWARE 0x08
#define PAGE_TYPE_FREE     0x10
#define PAGE_TYPE_INUSE    0x20
#define PAGE_TYPE_PGTBL    0x40
#define PAGE_TYPE_USER     0x80

struct page {
    unsigned int type;
    int          reference;
    void *       vaddr;
    int          reserv;
};

struct _block_list {
    struct _block_list *prev;
    struct _block_list *next;
};

typedef struct _block_list block_list;

struct memory_info {
#define MAX_ORDER 11
    block_list * free_list[MAX_ORDER]; // max block size is 2^(MAX_ORDER-1)=4MB
    size_t       free_count[MAX_ORDER];
    bitset *     buddy_map[MAX_ORDER];
    uint         memory_start;
    uint         memory_end;
    uint         usable_end; // buddy map located on the end of memory
    uint         page_count; // only memory_start to memory_end
    struct page *pages_info;
};

#define GET_PAGE_BY_ID(mem, id)   (((mem)->memory_start + ((id)*PG_SIZE)))
#define GET_ID_BY_PAGE(mem, page) ((page - ((mem)->memory_start)) / PG_SIZE)

struct memory_info mem_info;
// in pages, not in bytes
#define page_alloc(pages, attr)                                                \
    allocate_pages_of_power_2(&mem_info, round_up_power_2(pages), attr)
#define page_free(p, pages)                                                    \
    free_pages_of_power_2(&mem_info, (char *)p, round_up_power_2(pages))

static inline uint round_down_power_2(uint x) {
    if (x == 0)
        return 0;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x - (x >> 1);
}

static inline uint round_up_power_2(uint x) {
    x--;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x + 1;
}

static inline uint trailing_zero(uint x) {
    uint c;
    asm volatile("bsfl %0, %0" : "=a"(c) : "0"(x));
    return c;
}

static block_list *remove_from_free_list(struct memory_info *mem, block_list *p,
                                         uint order) {
    if (p->prev) {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    } else {
        if (mem->free_list[order] != p)
            panic("Block has no prev but not the first.");
        mem->free_list[order] = p->next;
        p->next->prev         = NULL;
    }
    p->next = NULL;
    p->prev = NULL;
    return p;
}

static block_list *attach_to_free_list(struct memory_info *mem, block_list *p,
                                       uint order) {
    // must attach a single node
    assert(p->prev == NULL);
    assert(p->next == NULL);
    p->next               = mem->free_list[order];
    p->next->prev         = p;
    p->prev               = NULL;
    mem->free_list[order] = p;
    return p;
}

static inline int xor_buddy_map(struct memory_info *mem, char *p, uint order) {
    uint page_idx = ((uint)p - mem->memory_start) / PG_SIZE;
    xor_bit(mem->buddy_map[order], page_idx >> (order + 1), 1);
    return check_bit(mem->buddy_map[order], page_idx >> (order + 1));
}

static void set_page_attr(struct memory_info *mem, char *page, size_t pg_count,
                          uint attr) {
    for (uint i = 0; i < pg_count; i++)
        mem->pages_info[GET_ID_BY_PAGE(mem, ((uint)page + i * PG_SIZE))].type =
            attr;
}

static void clear_page_info(struct memory_info *mem, char *page,
                            size_t pg_count, uint attr) {
    for (uint i = 0; i < pg_count; i++) {
        struct page *p =
            &mem->pages_info[GET_ID_BY_PAGE(mem, ((uint)page + i * PG_SIZE))];
        p->type      = attr;
        p->vaddr     = 0;
        p->reference = 0;
    }
}

static char *allocate_pages_of_power_2(struct memory_info *mem, uint order,
                                       uint attr) {
    if (order >= MAX_ORDER)
        return NULL;
    char *block = NULL;
    if (mem->free_count[order] == 0) {
        block = allocate_pages_of_power_2(mem, order + 1, 0);
        if (block == NULL)
            return NULL;
        attach_to_free_list(mem, (block_list *)block, order);
        mem->free_count[order]++;
        /* printf("= PUT %x into order %d free list =\n", block); */
        block += ((1 << order) * PG_SIZE);
        xor_buddy_map(mem, block, order); // higher half is returned
    } else {
        block =
            (char *)remove_from_free_list(mem, mem->free_list[order], order);
        mem->free_count[order]--;
        xor_buddy_map(mem, block, order);
    }
    if (block != NULL)
        set_page_attr(mem, block, 1 << order, attr);
    return block;
}

static int free_pages_of_power_2(struct memory_info *mem, char *p, uint order) {
    if (p == NULL)
        return 1; // free a NULL block
    if (!((uint)p >= mem->memory_start && (uint)p <= mem->usable_end))
        return 2; // free a block not managed by us
    uint buddy_bit = xor_buddy_map(mem, p, order);
    if (buddy_bit == 0 && order + 1 < MAX_ORDER) {
        char *buddy               = NULL;
        uint  page_idx            = ((uint)p - mem->memory_start) / PG_SIZE;
        uint  buddy_even_page_idx = (page_idx >> (order + 1)) << (order + 1);
        if (page_idx == buddy_even_page_idx)
            buddy = p + (1 << order) * PG_SIZE;
        else
            buddy = p - (1 << order) * PG_SIZE;
        // printf("= REMOVE %x from order %d free list =\n", buddy, order);
        remove_from_free_list(mem, (block_list *)buddy, order);
        mem->free_count[order]--;
        if (buddy < p)
            free_pages_of_power_2(mem, buddy, order + 1);
        else
            free_pages_of_power_2(mem, p, order + 1);
    } else {
        attach_to_free_list(mem, (block_list *)p, order);
        clear_page_info(mem, p, 1 << order, PAGE_TYPE_USABLE | PAGE_TYPE_FREE);
        mem->free_count[order]++;
    }
    return 0;
}

void print_free_info(struct memory_info *mem) {
    printf("[MEM] free blocks count is\n[MEM] ");
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("%04d, ", 1 << i);
    printf("\n[MEM] ");
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("%04d, ", mem->free_count[i]);
    printf("\n[MEM ]");
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("0x%x,", mem->free_list[i]);
    printf("\n");
}

// return NULL if cannot find.
static pte_t *get_pte(pde_t *page_dir, void *va) {
    pte_t *pg_tab = NULL;
    pde_t  pde    = page_dir[(uint)va >> 22];
    if (!(pde & PG_Present))
        return NULL;
    pg_tab = (pte_t *)(pde & ~0xFFF);
    return &pg_tab[((uint)va >> 12) & 0x3FF];
}

static pte_t *create_page_table(pde_t *page_dir, void *va, uint pde_attr) {
    pte_t *pte = get_pte(page_dir, va);
    pde_t *pde = &page_dir[(uint)va >> 22];
    if (pte) {
        mem_info.pages_info[GET_ID_BY_PAGE(&mem_info, (uint)(*pde & ~0xFFF))]
            .reference++;
        return pte;
    }
    pte_t *pg_tab = (pte_t *)page_alloc(1, PAGE_TYPE_INUSE | PAGE_TYPE_PGTBL);
    if (pg_tab == NULL)
        return NULL;
    // if reference not 1, it definitly wrong
    mem_info.pages_info[GET_ID_BY_PAGE(&mem_info, (uint)pg_tab)].reference = 1;
    memset(pg_tab, 0, PG_SIZE);
    // memory proc cr3 is directly mapped, no need for V2P
    *pde = (uint)pg_tab | PG_Present | pde_attr;
    return &pg_tab[((uint)va >> 12) & 0x3FF];
}

// size in bytes
static int map_pages(pde_t *page_dir, void *va, void *pa, size_t size,
                     uint pte_attr) {
    va              = (void *)PGROUNDDOWN((uint)va);
    void * end      = (void *)PGROUNDDOWN(((uint)va) + size - 1);
    pte_t *pte      = NULL;
    uint   pde_attr = PG_Present | PG_User | PG_Writeable;
    if (pte_attr & PG_OS_SYS)
        pde_attr = PG_Present | PG_OS_SYS;
    /* printf("Map %x-%x to %x\n", va, end, pa); */
    for (; (uint)va <= (uint)end; va += PG_SIZE, pa += PG_SIZE) {
        if ((pte = create_page_table(page_dir, va, pde_attr)) == NULL)
            return -1;
        if (*pte & PG_Present)
            panic("memory remap");
        *pte = (uint)pa | PG_Present | pte_attr;
        if ((uint)pa < mem_info.memory_start || (uint)pa > mem_info.usable_end)
            continue;
        mem_info.pages_info[GET_ID_BY_PAGE(&mem_info, (uint)pa)].reference++;
        mem_info.pages_info[GET_ID_BY_PAGE(&mem_info, (uint)pa)].vaddr = va;
    }
    return 0;
}

// size in bytes
static int unmap_pages(pde_t *page_dir, void *va, size_t size) {
    va                   = (void *)PGROUNDDOWN((uint)va);
    void * end           = (void *)PGROUNDDOWN(((uint)va) + size - 1);
    pte_t *pte           = NULL;
    uint   contiunos_pte = 0;
    for (; (uint)va <= (uint)end; va += PG_SIZE) {
        mem_info
            .pages_info[GET_ID_BY_PAGE(&mem_info,
                                       (uint)(vir2phy(page_dir, va)))]
            .reference--;
        pte = get_pte(page_dir, va);
        if (!(*pte & PG_Present))
            panic("memory not map");
        char *pa = (char *)((*pte & ~0xFFF) + ((unsigned int)va & 0xFFF));
        mem_info.pages_info[GET_ID_BY_PAGE(&mem_info, (uint)pa)].reference--;
        mem_info.pages_info[GET_ID_BY_PAGE(&mem_info, (uint)pa)].vaddr = 0;

        *pte = 0;
        if (((uint)va & 0x3FF000) == 0)
            contiunos_pte = 0;
        contiunos_pte++;
        if (contiunos_pte == PTE_PER_PDE) {
            // free 1024 pte entities, then free the page dir
            pde_t *pde = &page_dir[(uint)va >> 22];
            if (!(*pde & PG_Present))
                panic("page dir entity not mapped wtf?");
            pte_t *pg_tab = (pte_t *)((*pde) & ~0xFFF);
            if (--mem_info.pages_info[GET_ID_BY_PAGE(&mem_info, (uint)pg_tab)]
                      .reference == 0)
                page_free(pg_tab, 1);
            contiunos_pte = 0;
        }
    }

    return 0;
}

static pde_t *create_page_dir(void) {
    pde_t *page_dir = (pde_t *)page_alloc(1, PAGE_TYPE_INUSE | PAGE_TYPE_PGTBL);
    if (page_dir == NULL)
        return NULL;
    memset(page_dir, 0, PG_SIZE);
    size_t sys_map_size = sizeof(sys_mapping) / sizeof(sys_mapping[0]);
    for (uint i = 0; i < sys_map_size; i++)
        map_pages(page_dir, (void *)sys_mapping[i][1],
                  (void *)sys_mapping[i][0], sys_mapping[i][2],
                  PG_Present | PG_OS_SYS);
    /* page_dir[KERN_BASE >> 22] = 0 | PG_Present | PG_PS; */
    return page_dir;
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
        attach_to_free_list(mem, current, MAX_ORDER - 1);
        mem->free_count[MAX_ORDER - 1]++;
    }
    printf("[MEM] pages info size: %d KB\n",
           pg_count * sizeof(struct page) / 1024);
    for (uint i = 0; i < pg_count; i++) {
        if (GET_PAGE_BY_ID(mem, i) < mem->usable_end)
            mem->pages_info[i].type = PAGE_TYPE_FREE | PAGE_TYPE_USABLE;
        else
            mem->pages_info[i].type = PAGE_TYPE_INUSE | PAGE_TYPE_SYSTEM;
        mem->pages_info[i].vaddr = (void *)0xFFFFFFFF;
    }
    printf("[MEM] Initialized. Total %d pages. 4M Block Count: %d.\n", pg_count,
           mem->free_count[MAX_ORDER - 1]);
    print_free_info(mem);
    return;
}

// return child proc's pid
static uint fork_proc(pid_t pid) {
    process *parent_proc = get_proc(pid);
    parent_proc->status |= PROC_STATUS_STOP; // stop to wait to be forked
    process *child_proc = alloc_proc();
    if (child_proc == NULL)
        return 0;
    child_proc->parent_pid = pid;
    memcpy(&child_proc->stack, &parent_proc->stack, sizeof(stack_frame));
    child_proc->pstack                             = parent_proc->pstack;
    child_proc->pstack_size                        = parent_proc->pstack_size;
    child_proc->quene_body                         = NULL;
    child_proc->quene_head_sending_to_this_process = NULL;
    // child_proc pmsg is pa of (va of(par pmsg))
    child_proc->p_msg    = parent_proc->p_msg;
    child_proc->page_dir = create_page_dir();
    map_pages(child_proc->page_dir, child_proc->pstack,
              page_alloc(child_proc->pstack_size / PG_SIZE,
                         PAGE_TYPE_INUSE | PAGE_TYPE_USER),
              child_proc->pstack_size, PG_Present | PG_Writeable | PG_User);
    memcpy(vir2phy(child_proc->page_dir, child_proc->pstack),
           vir2phy(parent_proc->page_dir, parent_proc->pstack),
           child_proc->pstack_size);

    parent_proc->status &= ~PROC_STATUS_STOP;
    child_proc->status = parent_proc->status;

    return child_proc->pid;
}

static process *set_proc_exit(pid_t pid, uint exit_status) {
    process *proc     = get_proc(pid);
    proc->exit_status = exit_status;
    proc->status      = PROC_STATUS_HANGING | PROC_STATUS_STOP;
    return proc;
}

static void send_exit_to_parent(process *proc) {
    message msg;
    msg.type             = 0x12344321;
    msg.major            = proc->exit_status;
    msg.data.uint_arr.d1 = proc->pid;
    msg.receiver         = proc->parent_pid;
    send_msg(&msg);
    exit_proc(proc->pid);
}

static void free_proc(struct memory_info *mem, process *proc) {
    char * pstack  = proc->pstack;
    size_t ps_size = proc->pstack_size;
    if ((uint)pstack >= mem->memory_start && (uint)pstack <= mem->usable_end) {
        unmap_pages(proc->page_dir, pstack, ps_size);
        page_free(pstack, ps_size / PG_SIZE);
        proc->pstack = NULL;
    }
    // unmap others
    for (uint i = 0; i < 1024; i++) {
        if ((proc->page_dir[i] & PG_Present) &&
            !(proc->page_dir[i] & PG_OS_SYS)) {
            pte_t *ptes = (pte_t *)(proc->page_dir[i] & ~0xFFF);
            for (uint j = 0; j < PTE_PER_PDE; j++) {
                if (ptes[j] & PG_Present)
                    unmap_pages(proc->page_dir, (void *)(ptes[j] & ~0xFFF),
                                PG_SIZE);
            }
        }
    }
    // free others

    if (proc->parent_pid == 0)
        panic(" Init proc cannot exit. ");
    process *parent_proc = get_proc(proc->parent_pid);
    if (parent_proc->status & PROC_STATUS_WATING) {
        parent_proc->status &= ~PROC_STATUS_WATING;
        send_exit_to_parent(proc);
    } else {
        proc->status |= PROC_STATUS_HANGING;
    }

    process *p    = get_proc_list_head();
    process *init = get_proc(1);
    while (p) {
        if (p->parent_pid == proc->pid) {
            p->parent_pid = 1; // assign to init proc
            if ((p->status & PROC_STATUS_HANGING) &&
                (init->status & PROC_STATUS_WATING)) {
                init->status &= ~PROC_STATUS_WATING;
                send_exit_to_parent(p);
            }
        }
        p = p->next;
    }
}

void wait_proc(pid_t parent_pid) {
    process *parent_proc = get_proc(parent_pid);
    process *p           = get_proc_list_head();
    uint     children    = 0;
    while (p) {
        if (p->parent_pid == parent_pid) {
            children++;
            if (p->status & PROC_STATUS_HANGING) {
                send_exit_to_parent(p);
                return;
            }
        }
        p = p->next;
    }

    if (children)
        parent_proc->status |= PROC_STATUS_WATING;
    else {
        message msg;
        msg.type     = 0x43211234;
        msg.major    = 1;
        msg.receiver = parent_pid;
        send_msg(&msg);
    }
}

extern void init(void); // init.c

static void start_up_init() {
    process *init_proc = get_proc(1);
    init_proc->status  = PROC_STATUS_STOP;

    init_proc->stack.cs = SEL_CODE_DPL1;
    init_proc->stack.ds = SEL_DATA_DPL1;
    init_proc->stack.es = SEL_DATA_DPL1;
    init_proc->stack.fs = SEL_DATA_DPL1;
    init_proc->stack.gs = SEL_DATA_DPL1;
    init_proc->stack.ss = SEL_DATA_DPL1;

    init_proc->stack.eip = (u32)init;
    init_proc->pstack = page_alloc(2, PAGE_TYPE_INUSE | PAGE_TYPE_USER); // 8KB
    init_proc->pstack_size = 2 * PG_SIZE;
    init_proc->stack.esp = (uint)init_proc->pstack + init_proc->pstack_size - 1;
    if (init_proc->stack.esp >= mem_info.usable_end) {
        printf("\n\n  Allocation overflow!  \n\n");
        panic("Allocation overflow!");
    }
    printf("usable end is %x\n", mem_info.usable_end);
    init_proc->stack.eflags = 0x1202;
    init_proc->page_dir     = create_page_dir();
    map_pages((pde_t *)init_proc->page_dir, init_proc->pstack,
              init_proc->pstack, init_proc->pstack_size,
              PG_Present | PG_Writeable | PG_User);
    printf("Init Init proc with pd: %x\n", init_proc->page_dir);
    printf("esp is %x, mapped to %x\n", init_proc->stack.esp,
           vir2phy(init_proc->page_dir, (char *)init_proc->stack.esp));
    printf("Entry is %x\n",
           vir2phy(init_proc->page_dir, (char *)init_proc->stack.eip));

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

    printf("Allocated proc is %d\n", init_proc->pid);
}

void do_page_fault(stack_frame *intf) {
    uint va = intf->trap_no;
    kprintf("Proc %d triggered PageFault with cr2 as %x.\n",
            ((process *)intf)->pid, va);
    map_pages(((process *)intf)->page_dir, (char *)va,
              page_alloc(1, PG_User | PG_Writeable | PG_Present), PG_SIZE,
              PG_User | PG_Writeable | PG_Present);
    ((process *)intf)->status &= ~PROC_STATUS_ERROR;
}

void Task_Memory(void) {
    if (reg_proc("TaskMM") != 0)
        printf("[MEM] Cannot register as TaskMM\n");
    reg_exc_msg(EXCEPTION_PF);
    struct core_env_memory_zone zone[10];
    size_t                   zone_count = query_env(ENV_KEY_MMAP, (ubyte *)zone,
                                  sizeof(struct core_env_memory_zone) * 10);
    struct core_memory_usage core_usage;
    query_env(ENV_KEY_MEMORY_USAGE, (ubyte *)&core_usage, sizeof(core_usage));

    /* struct page *pages_info = (struct page *)core_usage.core_space_end; */

    mem_info.memory_start = (uint)KV2P(core_usage.core_space_end);
    mem_info.memory_end   = core_usage.memory_end;

    init_memory(&mem_info);
    /*
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("%x, ", mem_info.free_list[i]);
    printf("\n");

    char *test = allocate_pages_of_power_2(&mem_info, trailing_zero(128));
    printf("Allocated pages at 0x%x\n", test);
    print_free_info(&mem_info);
    free_pages_of_power_2(&mem_info, test, trailing_zero(128));
    print_free_info(&mem_info);
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("%x, ", mem_info.free_list[i]);
    printf("\n");
    */
    // 每一个进程都有自己的普通栈，大小应为4MB。系统初期的进程只有4KB（一页大小）
    // 系统映射在0x80000000之后的4MB内，这是全局的映射，用户进程有此map但是不能访问（权限过低）

    start_up_init(); // system startup
    /* printf("Sizeof proc is %d bytes\n", sizeof(process)); */
    extern struct core_env core_env;
    printf("Process total: %d\n", core_env.proc_max);

    message msg;
    while (1) {
        recv_msg(&msg, PROC_ANY);
        switch (msg.type) {
        case MSG_EXCEPTION:
            if (msg.major == 14)
                do_page_fault((stack_frame *)msg.data.uint_arr.d1);
            break;
        case MEM_ALLOC_PAGES:
            // TODO: alloc and map pages, use proc's cr3 V2P
            msg.major = (uint)allocate_pages_of_power_2(
                &mem_info, round_up_power_2(msg.major),
                PAGE_TYPE_INUSE | PAGE_TYPE_USER);
            SEND_BACK(msg);
            break;
        case MEM_FREE_PAGES:
            // TODO: unmap pages and free, V2P
            free_pages_of_power_2(&mem_info, (char *)msg.data.uint_arr.d1,
                                  round_up_power_2(msg.major));
            msg.major = 0;
            SEND_BACK(msg);
            break;
        case MEM_FORK_PROC:
            msg.major    = fork_proc(msg.sender);
            msg.receiver = msg.sender;
            if (msg.major == 0) {
                msg.major = -1;
                send_msg(&msg); // send to parent proc
                break;          // no child alloc proc failed
            }
            send_msg(&msg); // send to parent_proc
            msg.receiver = msg.major;
            msg.major    = 0;
            send_msg(&msg); // send to child proc
            break;
        case MEM_DESTROY_PROC: {
            pid_t proc = msg.sender;
            free_proc(&mem_info, set_proc_exit(msg.sender, msg.major));
            break;
        }
        case MEM_WAIT_PROC: {
            wait_proc(msg.sender);
            break;
        }
        default:
            break;
        }
    }
}
