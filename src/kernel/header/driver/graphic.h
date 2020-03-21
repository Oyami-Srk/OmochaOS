#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "generic/typedefs.h"

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE   (SCREEN_WIDTH * SCREEN_HEIGHT)

#define BLACK  0x0
#define BLUE   0x1
#define GREEN  0x2
#define RED    0x4
#define WHITE  0x7
#define BRIGHT 0x8
#define FLASH  0x80

#define COLOR(bg, fg) ((bg << 4) | fg)

void kprintf(const char *fmt, ...);
uint GRAPHIC_write_color_string_to_vm(u32 offset, u16 color, const char *str);
void GRAPHIC_set_vm_display_addr(uint offset);
void GRAPHIC_set_cursor_addr(uint offset);
void GRAPHIC_init(void);

#endif // __GRAPHIC_H__
