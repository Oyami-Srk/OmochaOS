#include "driver/graphic.h"
#include "generic/asm.h"
#include "lib/stdlib.h"

const volatile char *vm_start = (volatile char *)0x800B8000;
uint                 disp_pos = 0;

void kprintf(const char *fmt, ...) {
    int     i;
    char    buf[256];
    va_list arg = (va_list)((char *)(&fmt) + 4);
    i           = vsprintf(buf, fmt, arg);
    buf[i]      = 0;
    disp_pos    = GRAPHIC_write_color_string_to_vm(
        disp_pos, COLOR(BLACK, WHITE | BRIGHT), buf);
}

uint GRAPHIC_write_color_string_to_vm(u32 offset, u16 color, const char *str) {
    volatile char *pCh = (volatile char *)((uint)vm_start + offset);
    char           ch  = *str;
    while (*str != 0) {
        switch (ch) {
        case '\n':
            pCh =
                (volatile char *)(vm_start +
                                  SCREEN_WIDTH * 2 *
                                      (((uint)pCh - (uint)vm_start) / 160 + 1));
            break;
        case '\b':
            if (pCh - 2 < vm_start)
                break;
            *(pCh - 2) = ' ';
            *(pCh - 1) = color;
            pCh -= 2;
            break;
        default:
            *pCh++ = ch;
            *pCh++ = color;
            break;
        }
        ch = *++str;
    }
    if (*(pCh + 1) == 0)
        *(pCh + 1) = color; // make cursor visible
    return (uint)pCh - (uint)vm_start;
}

void panic_proto(const char *str, const char *s_fn, const char *b_fn,
                 const int ln) {
    // check cs for privilege
    uint cs = 0;
    asm("mov %%cs, %0" : "=r"(cs));
    // TODO: set panic int to receive panic from user processes
    if ((uint)(cs & 3) != 0) { // you cannot panic in ring1-3, please use panic
                               // int pass message to kernel
#ifdef __DEBUG
        ;
#else
        return;
#endif
    }
    asm volatile("cli"); // close interrupt
    GRAPHIC_write_color_string_to_vm(0, COLOR(BLUE, BLACK),
                                     "                    "
                                     "                    "
                                     "                    "
                                     "                   ");
    char  buf[4];
    char *p_str = (char *)str;
    uint  i     = 0;
    i = GRAPHIC_write_color_string_to_vm(i, COLOR(BLUE, WHITE), "[PANIC] ");
    i = GRAPHIC_write_color_string_to_vm(i, COLOR(BLUE, WHITE), p_str);
    i = GRAPHIC_write_color_string_to_vm(i, COLOR(BLUE, WHITE), " in line ");
    itoa(ln, buf, 10);
    i = GRAPHIC_write_color_string_to_vm(i, COLOR(BLUE, WHITE), buf);
    i = GRAPHIC_write_color_string_to_vm(i, COLOR(BLUE, WHITE), " of ");
    i = GRAPHIC_write_color_string_to_vm(i, COLOR(BLUE, WHITE), s_fn);
    i = GRAPHIC_write_color_string_to_vm(i, COLOR(BLUE, WHITE), " based on ");
    i = GRAPHIC_write_color_string_to_vm(i, COLOR(BLUE, WHITE), b_fn);
    magic_break();
    while (1)
        ;
}
