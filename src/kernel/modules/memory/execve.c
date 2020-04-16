#include "buddy.h"
#include "core/memory.h"
#include "core/process.h"
#include "generic/asm.h"
#include "generic/typedefs.h"
#include "lib/elf.h"
#include "memory.h"
#include "modules/fs.h"
#include "paging.h"

#include "driver/graphic.h"
#include "modules/tty.h"

// counts the str array ending in NULL, NULL is not included
static int count_strs(const char **strs) {
    uint r = 0;
    for (; strs[r] != NULL; r++)
        ;
    return r;
}

#define ERROR(s) printf("[ERR] " s)

BOOL elf_check_file(Elf32_Ehdr *hdr) {
    if (!hdr)
        return FALSE;
    if (hdr->e_ident[EI_MAG0] != ELFMAG0) {
        ERROR("ELF Header EI_MAG0 incorrect.\n");
        return FALSE;
    }
    if (hdr->e_ident[EI_MAG1] != ELFMAG1) {
        ERROR("ELF Header EI_MAG1 incorrect.\n");
        return FALSE;
    }
    if (hdr->e_ident[EI_MAG2] != ELFMAG2) {
        ERROR("ELF Header EI_MAG2 incorrect.\n");
        return FALSE;
    }
    if (hdr->e_ident[EI_MAG3] != ELFMAG3) {
        ERROR("ELF Header EI_MAG3 incorrect.\n");
        return FALSE;
    }
    return TRUE;
}

BOOL elf_check_supported(Elf32_Ehdr *hdr) {
    if (!elf_check_file(hdr)) {
        ERROR("Invalid ELF File.\n");
        return FALSE;
    }
    if (hdr->e_ident[EI_CLASS] != ELFCLASS32) {
        ERROR("Unsupported ELF File Class.\n");
        return FALSE;
    }
    if (hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        ERROR("Unsupported ELF File byte order.\n");
        return FALSE;
    }
    if (hdr->e_machine != EM_386) {
        ERROR("Unsupported ELF File target.\n");
        return FALSE;
    }
    if (hdr->e_ident[EI_VERSION] != EV_CURRENT) {
        ERROR("Unsupported ELF File version.\n");
        return FALSE;
    }
    if (/*hdr->e_type != ET_REL && */ hdr->e_type != ET_EXEC) {
        ERROR("Unsupported ELF File type(only exec type supported).\n");
        return FALSE;
    }
    return TRUE;
}

static inline char *get_offsetd_addr(char *base, char *addr, char *vbase) {
    return (vbase + (addr - base));
}

void mem_execve(struct memory_info *mem, process *caller, const char *exec_fn,
                const char *argv[], const char *env[]) {
    char *pstack_top    = (char *)KERN_BASE;
    char *pstack_bottom = (char *)(KERN_BASE - PROC_STACK_SIZE);
    // copy fn
    exec_fn    = (const char *)vir2phy(caller->page_dir, (char *)exec_fn);
    size_t fns = strlen(exec_fn) + 1;
    char * fn  = mem_kmalloc(fns + 1);
    memcpy(fn, exec_fn, fns + 1);

    char *stack = page_alloc(mem, PROC_STACK_SIZE / PG_SIZE,
                             PAGE_TYPE_INUSE | PAGE_TYPE_USER);
    // setup satck
    env  = (const char **)vir2phy(caller->page_dir, (char *)env);
    argv = (const char **)vir2phy(caller->page_dir, (char *)argv);

    char *sp           = stack + PROC_STACK_SIZE;
    char *head_of_argv = NULL;
    char *head_of_envp = NULL;

    int argc = count_strs(argv);
    int envc = count_strs(env);

    // setup stack for envp
    for (int i = envc - 1; i >= 0; i--) {
        const char *e = (const char *)vir2phy(caller->page_dir, (char *)env[i]);
        size_t      len = strlen(e);
        sp -= len + 1;
        memcpy(sp, e, len + 1);
    }
    head_of_envp = sp;
    // setup stack for argv
    for (int i = argc - 1; i >= 0; i--) {
        const char *e =
            (const char *)vir2phy(caller->page_dir, (char *)argv[i]);
        size_t len = strlen(e);
        sp -= len + 1;
        memcpy(sp, e, len + 1);
    }
    head_of_argv = sp;
    sp           = (char *)ROUNDDOWN_WITH(sizeof(uint), (uint)sp); // padding sp
    // setup stack for ELF aux, not impl yet

    // setup envp
    sp -= sizeof(char *) * (envc + 1);
    char **envp = (char **)sp;
    // envp[0]     = head_of_envp;
    envp[0] = get_offsetd_addr(stack, head_of_envp, pstack_bottom);
    for (uint i = 1; i <= envc; i++) {
        if (i == envc)
            envp[i] = NULL;
        else {
            const char *e =
                (const char *)vir2phy(caller->page_dir, (char *)env[i - 1]);
            size_t len = strlen(e);
            envp[i]    = envp[i - 1] + len + 1;
        }
    }
    // setup argv
    sp -= sizeof(char *) * (argc + 1);
    char **argvp = (char **)sp;
    // argvp[0]     = head_of_argv;
    argvp[0] = get_offsetd_addr(stack, head_of_argv, pstack_bottom);
    for (uint i = 1; i <= argc; i++) {
        if (i == argc)
            argvp[i] = NULL;
        else {
            const char *e =
                (const char *)vir2phy(caller->page_dir, (char *)argv[i - 1]);
            size_t len = strlen(e);
            argvp[i]   = argvp[i - 1] + len + 1;
        }
    }

    uint offset_sp = stack - sp;

    caller->status = PROC_STATUS_STOP;
    free_proc(mem, caller);
    caller->page_dir    = create_page_dir(mem);
    caller->pstack      = pstack_bottom;
    caller->pstack_size = PROC_STACK_SIZE;
    map_pages(mem, caller->page_dir, caller->pstack, stack, caller->pstack_size,
              PG_Present | PG_Writable | PG_User);
    caller->stack.esp = ROUNDDOWN_WITH(
        sizeof(uint), (uint)pstack_bottom - offset_sp); // bound down
    caller->stack.ecx = argc;
    caller->stack.ebx = (uint)pstack_bottom - offset_sp;

    // read exec file
    struct fs_file_info file_info;
    memset(&file_info, 0, sizeof(struct fs_file_info));
    FS_get_file_info(fn, &file_info);
    char   proc_name[16];
    size_t exe_len = strlen(file_info.filename);
    memset(proc_name, 0, 16);
    int   current_no = 0;
    pid_t pid        = 0;
    do {
        message msg;
        msg.type     = QUERY_PROC;
        msg.receiver = SYSTASK_PID;
        send_msg(&msg);
        recv_msg(&msg, SYSTASK_PID);
        pid = msg.major;
        current_no++;
    } while (pid);

    if (current_no != 1) {
        if (current_no > 1000000000) {
            ERROR("too many same exec name");
            goto kill;
        }
        static int p10[]  = {1,      10,      100,      1000,      10000,
                            100000, 1000000, 10000000, 100000000, 1000000000};
        int        max_no = p10[(15 - exe_len)];
        if (current_no > max_no) {
            ERROR("too many same exec name");
            goto kill;
        }
        sprintf(proc_name, "%s%d", file_info.filename, current_no);

    } else {
        memcpy(proc_name, file_info.filename, exe_len + 1);
    }
    memcpy(caller->name, proc_name, 16);

    Elf32_Ehdr elf_header;
    FS_read_file(&file_info, 0, &elf_header, sizeof(Elf32_Ehdr));
    // printf("Load file as proc[%s] : %s, size: %d bytes\n", proc_name,
    //    file_info.filename, file_info.size);
    BOOL is_vailed = elf_check_supported(&elf_header);
    if (!is_vailed) {
    // kill proc
    kill:
        caller->exit_status = 255;
        caller->status |= PROC_STATUS_HANGING | PROC_STATUS_ERROR;
        mem_exit_proc(caller);
        return;
    }
    struct prog_info *pgi = caller->prog_info =
        (struct prog_info *)mem_kmalloc(sizeof(struct prog_info));

    caller->prog_info->image_start = (char *)0xFFFFFFFF;

    // load prog header
    assert(sizeof(Elf32_Phdr) == elf_header.e_phentsize);
    assert(sizeof(Elf32_Shdr) == elf_header.e_shentsize);
    Elf32_Phdr *prog_header =
        (Elf32_Phdr *)mem_kmalloc(elf_header.e_phnum * sizeof(Elf32_Phdr));
    FS_read_file(&file_info, elf_header.e_phoff, prog_header,
                 sizeof(Elf32_Phdr) * elf_header.e_phnum);
    for (uint i = 0; i < elf_header.e_phnum; i++) {
        if (prog_header[i].p_type == PT_LOAD) {
            if ((uint)pgi->image_start > prog_header[i].p_vaddr) {
                pgi->image_start = (char *)prog_header[i].p_vaddr;
            }
            pgi->program_size += prog_header[i].p_memsz;
            size_t memsz = prog_header[i].p_memsz;
            memsz        = PGROUNDUP(memsz);

            char *pa        = page_alloc(mem, memsz / PG_SIZE,
                                  PAGE_TYPE_INUSE | PAGE_TYPE_USER);
            char *va        = (char *)prog_header[i].p_vaddr;
            uint  va_offset = (uint)va - PGROUNDDOWN((uint)va);

            FS_read_file(&file_info, prog_header[i].p_offset, pa + va_offset,
                         prog_header[i].p_filesz);
            if (va_offset)
                memset(pa, 0, va_offset);
            if (prog_header[i].p_filesz != prog_header[i].p_memsz) {
                if (prog_header[i].p_filesz > prog_header[i].p_memsz) {
                    ERROR("Prog header filesz bigger than memsize");
                    magic_break();
                }
                memset(pa + va_offset + prog_header[i].p_filesz, 0,
                       prog_header[i].p_memsz - prog_header[i].p_filesz);
            }

            map_pages(
                mem, caller->page_dir, (void *)PGROUNDDOWN((uint)va), pa,
                prog_header[i].p_memsz,
                PG_Present | PG_User |
                    ((prog_header[i].p_flags & PF_W) != 0 ? PG_Writable : 0));
        }
    }

    pgi->program_break = pgi->image_start + pgi->program_size;
    caller->stack.eip  = elf_header.e_entry;
    mem_kfree(prog_header);

    caller->p_msg  = NULL;
    caller->status = PROC_STATUS_RUNNING;
}