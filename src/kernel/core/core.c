#include <core/interrupt.h>
#include <core/memory.h>
#include <core/multiboot.h>
#include <core/paging.h>
#include <core/process.h>
#include <core/protect.h>
#include <driver/graphic.h>
#include <driver/misc.h>
#include <generic/typedefs.h>
#include <lib/stdlib.h>

#include <lib/syscall.h>
#include <modules/modules.h>
#include <modules/systask.h>

#include <core/cpuid.h>
#include <core/environment.h>
#include <core/init.h>

#include <driver/driver.h>
#include <driver/port.h>

uint modules[__MODULES_COUNT__]               = __MODULES_ENTRIES__;
uint modules_preferred_pid[__MODULES_COUNT__] = __MODULES_PREFERRED_PID__;

void restart() {
    u8 good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
}

_Noreturn void SysIdle() {
    // reflect msg to sender
    message msg;
    while (1) {
        recv_msg(&msg, PROC_ANY);
        SEND_BACK(msg);
        /*
        asm("nop\n\t");
        */
    }
}

pde_t core_page_dir[PDE_SIZE];

struct core_env    core_env;
Driver_Declaration drivers = {
    .magic = DRIVER_DC_HEAD, .name = "HEAD", .level = -1};

#include <driver/ahci/ahci.h>

BOOL read(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count,
          uint16_t *buf);

u16             buf[512];
extern HBA_MEM *hba;

_Noreturn void core_main(multiboot_info_t *multiboot_header, u32 magic) {
    init_port();
    memset(&core_env, 0, sizeof(struct core_env));
    memcpy(&core_env.boot_info, multiboot_header, sizeof(multiboot_info_t));

    /*
     * ===   KERNEL CODE    === <- 0x80100000
     * === KERNEL CODE END  === <- KERN_VEND
     * === CORE SPACE START === aligned up 1MB
     *     (CORE FREE START)
     * ===  CORE FREE END   === <- FREE START + 1MB
     * ===  CORE SPACE END  === <- CODE + 4MB
     */

    core_env.beats     = 0;
    core_env.core_vend = (uint)KERN_VEND;
    core_env.core_space_start =
        ((uint)KERN_VEND & 0xFFF00000) + 0x100000; // align 1MB
    core_env.core_space_free_start = core_env.core_space_start;
    core_env.core_space_free_end =
        core_env.core_space_free_start + 0x100000; // 1MB

    core_env.core_space_end = (uint)KP2V(16 * 1024 * 1024); // 16MB

    core_env.gdt_size = GDT_SIZE;
    core_env.idt_size = IDT_SIZE;
    core_env.page_dir = core_page_dir;

    core_init_memory(&core_env);

    memset((void *)core_env.core_space_start, 0,
           core_env.core_space_end - core_env.core_space_start);

    /*
    u32 fb_addr = multiboot_header->framebuffer_addr & 0xFFFFFFFF;
    GRAPHIC_init((void *)(fb_addr), multiboot_header->framebuffer_width,
                 multiboot_header->framebuffer_height,
                 multiboot_header->framebuffer_pitch);
                 */

    // prepare for load driver
    Driver_Declaration *dd_parent    = &drivers;
    uint                driver_count = 0;
    section_foreach_entry(DrvDclrList, Driver_Declaration *, entry) {
        // kprintf("%s,", (*entry)->name);
        dd_parent->next = *entry;
        dd_parent       = dd_parent->next;
        dd_parent->next = NULL;
        driver_count++;
    }
    drivers.major_ver = driver_count;
    core_init_driver(&core_env, &drivers);

    core_env.proc_table = (process *)core_env.core_space_free_end;
    size_t ideal_proc_max =
        ((core_env.core_space_end - core_env.core_space_free_end) /
         sizeof(process));
    size_t bitmap_size = ideal_proc_max / 8; // in bytes
    core_env.proc_max =
        ideal_proc_max - (bitmap_size / sizeof(process) +
                          (bitmap_size % sizeof(process) == 0 ? 0 : 1));
    core_env.proc_count  = 0;
    core_env.proc_bitmap = (bitset *)(core_env.core_space_end - bitmap_size);
    /* core_env.proc_bitmap_size = (bitmap_size / sizeof(bitset)); */
    // someof last pid cannot be used
    core_env.proc_bitmap_size = ((core_env.proc_max / 8) / sizeof(bitset));

    /*
    kprintf("\n\nproc_max: %d\nbitmap_size: %d\n", core_env.proc_max,
            core_env.proc_bitmap_size);
    kprintf("Proc table start: 0x%x\n", core_env.proc_table);
    kprintf("Bitmap start: 0x%x\n", core_env.proc_bitmap);
    */

    core_init_protect(&core_env);
    core_init_interrupt(&core_env);
    core_init_proc(&core_env);

    init_proc(0, SysIdle, core_page_dir);
    /* init_proc(1, SysIdle, (u32)core_page_dir); */
    set_bit(core_env.proc_bitmap, 1); // leave pid 1 to init
    for (uint i = 0; i < __MODULES_COUNT__; i++)
        if (modules_preferred_pid[i] < 0xFFFF)
            init_proc(modules_preferred_pid[i], (void *)modules[i],
                      core_page_dir);
    for (uint i = 0; i < __MODULES_COUNT__; i++)
        if (modules_preferred_pid[i] >= 0xFFFF)
            init_proc(modules_preferred_pid[i], (void *)modules[i],
                      core_page_dir);

    memset((void *)buf, 0xBF, 512 * 2);

    BOOL result = read(&hba->ports[0], 0, 0, 2, KV2P(buf));

    kprintf("result: %d, last is : 0x%x\n", result, buf[0]);
    for (int i = 0; i < 512; i++) {
        kprintf("%x ", buf[i]);
    }

    kprintfc(YELLOW, BLACK, "Jump to process.\n");
    move_to_proc();
    while (1)
        ;
}

// PDE enable large Page Size - 4MB
// clang-format off
__attribute__((__aligned__(PG_SIZE))) pde_t core_page_dir[PDE_SIZE] = {
    [0]               = (0) | PG_Present | PG_Writable | PG_PS,
    [1]               = (0x00400000) | PG_Present | PG_Writable | PG_PS,
    [2]               = (0x00800000) | PG_Present | PG_Writable | PG_PS | PG_PCD,
    [3]               = (0x00C00000) | PG_Present | PG_Writable | PG_PS,

    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writable | PG_PS | PG_User,
    [(KERN_BASE >> 22)+1] = (0x00400000) | PG_Present | PG_Writable | PG_PS,
    [(KERN_BASE >> 22)+2] = (0x00800000) | PG_Present | PG_Writable | PG_PS | PG_PCD,
    [(KERN_BASE >> 22)+3] = (0x00C00000) | PG_Present | PG_Writable | PG_PS,
};
// clang-format on
