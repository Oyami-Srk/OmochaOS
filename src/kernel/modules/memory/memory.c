#include "core/memory.h"
#include "buddy.h"
#include "core/environment.h"
#include "core/paging.h"
#include "generic/asm.h"
#include "lib/bitset.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"
#include "memory.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "modules/tty.h"

struct memory_info mem_info;
// in pages, not in bytes

extern char _init_elf_start_[];
extern char _init_elf_end_[];

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
    init_memory_pool(&mem_info, PG_SIZE * 1024);
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

    start_up_init(&mem_info, (ubyte *)_init_elf_start_); // system startup
    /* printf("Sizeof proc is %d bytes\n", sizeof(process)); */
    extern struct core_env core_env;
    printf("Process total: %d\n", core_env.proc_max);
    message msg;

    while (1) {
        recv_msg(&msg, PROC_ANY);
        switch (msg.type) {
        case MSG_EXCEPTION:
            if (msg.major == 14)
                do_page_fault(&mem_info, (stack_frame *)msg.data.uint_arr.d1);
            break;
        case MEM_ALLOC_PAGES:
            // TODO: alloc and map pages, use proc's cr3 V2P
            // msg.major = (uint)allocate_pages_of_power_2(
            // &mem_info, round_up_power_2(msg.major),
            // PAGE_TYPE_INUSE | PAGE_TYPE_USER);
            SEND_BACK(msg);
            break;
        case MEM_FREE_PAGES:
            // TODO: unmap pages and free, V2P
            // free_pages_of_power_2(&mem_info, (char *)msg.data.uint_arr.d1,
            //   round_up_power_2(msg.major));
            msg.major = 0;
            SEND_BACK(msg);
            break;
        case MEM_FORK_PROC: {
            process *child_proc;
            process *parent_proc;
            msg.major =
                fork_proc(&mem_info, msg.sender, &parent_proc, &child_proc);
            msg.receiver = msg.sender;
            if (msg.major == 0) {
                msg.major = -1;
                send_msg(&msg); // send to parent proc
                break;          // no child alloc proc failed
            }
            // magic_break();
            send_msg(&msg); // send to parent_proc
            msg.receiver = msg.major;
            msg.major    = 0;
            send_msg(&msg); // send to child proc
            parent_proc->status &= ~PROC_STATUS_STOP;
            child_proc->status &= ~PROC_STATUS_STOP;
            break;
        }
        case MEM_EXIT_PROC: {
            pid_t proc = msg.sender;
            mem_exit_proc(
                free_proc(&mem_info, set_proc_exit(msg.sender, msg.major)));
            break;
        }
        case MEM_WAIT_PROC: {
            wait_proc(msg.sender);
            break;
        }
        case MEM_FREE_PROC:
            free_proc(&mem_info, (process *)(msg.major));
            msg.major = 0;
            SEND_BACK(msg);
            break;
        case MEM_MEMALLOC: {
            char *mem = mem_kmalloc(msg.major);
            // assume caller has the same address space as memory_proc did
            if (vir2phy(get_proc(msg.sender)->page_dir, mem) != mem) {
                mem_kfree(mem);
                msg.major = NULL;
            } else
                msg.major = (uint)mem;
            SEND_BACK(msg);
            break;
        }
        case MEM_MEMFREE: {
            char *mem = (char *)msg.major;
            if (vir2phy(get_proc(msg.sender)->page_dir, mem) != mem) {
                panic("Free error.");
                magic_break();
                break;
            }
            mem_kfree(mem);
            break;
        }
        case MEM_EXECVE: {
            mem_execve(&mem_info, get_proc(msg.sender), (const char *)msg.major,
                       (const char **)msg.data.uint_arr.d1,
                       (const char **)msg.data.uint_arr.d2);
            break;
        }
        case MEM_SBRK: {
            msg.major =
                (uint)mem_sbrk(&mem_info, get_proc(msg.sender), (int)msg.major);
            SEND_BACK(msg);
            break;
        }
        default:
            break;
        }
    }
}
