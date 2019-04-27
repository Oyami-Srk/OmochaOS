#ifndef __VGA_H__
#define __VGA_H__

#include "kernel/type.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define RED 0x4
#define WHITE 0x7
#define FLASH 0x80
#define BRIGHT 0x08
#define COLOR(bg, fg) ((bg << 4) | fg)

void VGA_write_color_string_to_vm(uint offset, ushort color, const char *str);
void VGA_set_vm_display_addr(uint offset);
void VGA_set_cursor_addr(uint offset);
void VGA_init(void);

#endif // __VGA_H__