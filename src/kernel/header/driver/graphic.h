#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "generic/typedefs.h"

#define WHITE  0x00ffffff //白
#define BLACK  0x00000000 //黑
#define RED    0x00ff0000 //红
#define ORANGE 0x00ff8000 //橙
#define YELLOW 0x00ffff00 //黄
#define GREEN  0x0000ff00 //绿
#define BLUE   0x000000ff //蓝
#define INDIGO 0x0000ffff //靛
#define PURPLE 0x008000ff //紫

#define DEFAULT_FG WHITE
#define DEFAULT_BG BLACK

void kprintf(const char *fmt, ...);
void kprintfc(uint fg, uint bg, const char *fmt, ...);
void GRAPHIC_init(uint *fb, int width, int height, uint pitch);
void put_bitmap(uint *addr, size_t width_pitch, const unsigned char *bitmap,
                uint bitmap_X, uint bitmap_Y, uint Front_Color,
                uint Background_Color);
void kputc_color(char c, uint fg, uint bg);

#endif // __GRAPHIC_H__
