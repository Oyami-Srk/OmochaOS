#include "driver/graphic.h"
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
