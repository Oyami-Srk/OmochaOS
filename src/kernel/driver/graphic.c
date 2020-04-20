#include "driver/graphic.h"
#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/string.h"

#include "font_8x13.h"
#include "logo.h"

const volatile char *vm_start = (volatile char *)0x800B8000;
uint                 disp_pos = 0;

uint  disp_offset = 0; // in pixel
uint  disp_width  = 0;
uint  disp_height = 0;
uint  disp_pitch  = 0;
uint *fb_addr     = 0;

// bitmap X must be 8 multiple
void put_bitmap(uint *addr, size_t width_pitch, const unsigned char *bitmap,
                uint bitmap_X, uint bitmap_Y, uint Front_Color,
                uint Background_Color) {
    for (uint y = 0; y < bitmap_Y; y++) {
        for (uint i = 0; i < bitmap_X / 8; i++) {
            char t = 0x01;
            for (uint x = 0; x < 8; x++) {
                char c = *bitmap;
                if (c & t)
                    addr[x + i * 8] = Front_Color;
                else
                    addr[x + i * 8] = Background_Color;
                t <<= 1;
            }
            bitmap++;
        }
        addr += width_pitch / sizeof(uint);
    }
}

void kputc_color(char c, uint fg, uint bg) {
    if (c == '\n') {
        disp_offset += disp_width - disp_offset % disp_width;
        disp_offset += (FONT_HEIGHT - 1) * disp_width;
        return;
    }
    if (c == '\b') {
        disp_offset -= FONT_WIDTH + FONT_GAP_X;
        kputc_color(' ', 0x0, 0x0);
        disp_offset -= FONT_WIDTH + FONT_GAP_X;
        return;
    }

    if ((disp_offset % disp_width) + FONT_WIDTH > disp_width)
        kputc_color('\n', fg, bg);

    if (disp_offset > (disp_height - FONT_HEIGHT) * disp_width)
        disp_offset = 0;

    if (c - FONT_OFFSET > sizeof(font) / FONT_HEIGHT) {
        put_bitmap(fb_addr + disp_offset, disp_pitch, font_block, FONT_WIDTH,
                   FONT_HEIGHT, fg, bg);
    } else {
        put_bitmap(fb_addr + disp_offset, disp_pitch, font[c - FONT_OFFSET],
                   FONT_WIDTH, FONT_HEIGHT, fg, bg);
    }
    if (bg && FONT_GAP_X) {
        for (uint i = 0; i < FONT_HEIGHT; i++) {
            for (uint j = 0; j < FONT_GAP_X; j++) {
                *(fb_addr + disp_offset + disp_width * i + FONT_WIDTH + j) = bg;
            }
        }
    }
    disp_offset += FONT_WIDTH + FONT_GAP_X;
}

void kprintfc(uint fg, uint bg, const char *fmt, ...) {
    int     i;
    char    buf[256];
    va_list arg = (va_list)((char *)(&fmt) + 4);
    i           = vsprintf(buf, fmt, arg);
    buf[i]      = 0;

    char *str = buf;

    while (*str != 0) {
        kputc_color(*str, fg, bg);
        str++;
    }
}

void kprintf(const char *fmt, ...) {
    int     i;
    char    buf[256];
    va_list arg = (va_list)((char *)(&fmt) + 4);
    i           = vsprintf(buf, fmt, arg);
    buf[i]      = 0;

    char *str = buf;

    while (*str != 0) {
        kputc_color(*str, DEFAULT_FG, DEFAULT_BG);
        str++;
    }
}

void panic_proto(const char *str, const char *s_fn, const char *b_fn,
                 const int ln) {
    // check cs for privilege
    uint cs = 0;
    asm("mov %%cs, %0" : "=r"(cs));
    // TODO: set panic int to receive panic from user processes
    if ((uint)(cs & 3) != 0) { // you cannot panic in ring1-3, please use panic
                               // int pass message to kernel
#ifdef __DEBUG__
        ;
#else
        return;
#endif
    } else {
        asm volatile("cli"); // close interrupt for ring 0 panic
    }

    kprintfc(WHITE, BLUE, "[PANIC] %s in line %d of %s based on %s.\n", str, ln,
             s_fn, b_fn);
    magic_break();
    if ((uint)(cs & 3) == 0)
        while (1)
            ;
}

void GRAPHIC_init(uint *fb, int width, int height, uint pitch) {
    fb_addr     = fb;
    disp_width  = width;
    disp_height = height;
    disp_pitch  = pitch;
    for (uint y = 0; y < LOGO_Y; y++) {
        memcpy(fb_addr + y * disp_width, logo[y], sizeof(uint) * LOGO_X);
    }
    disp_offset = disp_width * (LOGO_Y + 5);
}
