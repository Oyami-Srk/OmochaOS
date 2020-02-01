#include "driver/graphic.h"

const volatile char *vm_start = (volatile char *)0xB8000;

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
