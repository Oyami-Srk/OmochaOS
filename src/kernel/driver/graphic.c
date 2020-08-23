#include <driver/graphic.h>
#include <generic/asm.h>
#include <lib/stdlib.h>
#include <lib/string.h>

#include <static/font_8x13.h>
#include <static/logo.h>

#include <driver/driver.h>

const volatile char *vm_start = (volatile char *)0x800B8000;
uint                 disp_pos = 0;

uint  disp_offset = 0; // in pixel
uint  char_width;
uint  char_height;
int   char_x; // point to next char space
int   char_y;
int   char_y_start;
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

void kputc_color_xy(int x, int y, char c, uint fg, uint bg) {
    if (c - FONT_OFFSET > sizeof(font) / FONT_HEIGHT) {
        put_bitmap(fb_addr + (x * (FONT_WIDTH + FONT_GAP_X)) +
                       (y * FONT_HEIGHT * disp_width),
                   disp_pitch, font_block, FONT_WIDTH, FONT_HEIGHT, fg, bg);
    } else {
        put_bitmap(fb_addr + (x * (FONT_WIDTH + FONT_GAP_X)) +
                       (y * FONT_HEIGHT * disp_width),
                   disp_pitch, font[c - FONT_OFFSET], FONT_WIDTH, FONT_HEIGHT,
                   fg, bg);
    }
}

void kputc_color(char c, uint fg, uint bg) {
    if (c == '\n') {
        char_y++;
        char_x = 0;
        if (char_y >= char_height)
            char_y = char_y_start;
        return;
    }
    if (c == '\b') {
        // disp_offset -= FONT_WIDTH + FONT_GAP_X;
        // disp_offset -= FONT_WIDTH + FONT_GAP_X;
        if (char_x == 0) {
            char_x = char_width - 1;
            if (char_y != 0)
                char_y--;
        } else
            char_x--;
        put_bitmap(fb_addr + (char_x * (FONT_WIDTH + FONT_GAP_X)) +
                       (char_y * FONT_HEIGHT * disp_width),
                   disp_pitch, font[' ' - FONT_OFFSET], FONT_WIDTH, FONT_HEIGHT,
                   fg, bg);
        return;
    }

    if (c - FONT_OFFSET > sizeof(font) / FONT_HEIGHT) {
        put_bitmap(fb_addr + (char_x * (FONT_WIDTH + FONT_GAP_X)) +
                       (char_y * FONT_HEIGHT * disp_width),
                   disp_pitch, font_block, FONT_WIDTH, FONT_HEIGHT, fg, bg);
    } else {
        put_bitmap(fb_addr + (char_x * (FONT_WIDTH + FONT_GAP_X)) +
                       (char_y * FONT_HEIGHT * disp_width),
                   disp_pitch, font[c - FONT_OFFSET], FONT_WIDTH, FONT_HEIGHT,
                   fg, bg);
    }

    if (bg && FONT_GAP_X) {
        for (uint i = 0; i < FONT_HEIGHT; i++) {
            for (uint j = 0; j < FONT_GAP_X; j++) {
                *(fb_addr +
                  ((char_x * (FONT_WIDTH + FONT_GAP_X)) +
                   (char_y * FONT_HEIGHT * disp_width)) +
                  disp_width * i + FONT_WIDTH + j) = bg;
            }
        }
    }

    if (++char_x >= char_width) {
        char_x = 0;
        if (++char_y >= char_height)
            char_y = char_y_start;
        return;
    }
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

void __GRAPHIC_init(uint *fb, int width, int height, uint pitch) {
    fb_addr     = fb;
    disp_width  = width;
    disp_height = height;
    disp_pitch  = pitch;

    char_width  = disp_width / (FONT_WIDTH + FONT_GAP_X);
    char_height = disp_height / FONT_HEIGHT;
    char_x      = 0;
    char_y      = 0;

    for (uint y = 0; y < LOGO_Y; y++) {
        memcpy(fb_addr + y * disp_width, logo[y], sizeof(uint) * LOGO_X);
    }
    disp_offset  = disp_width * (LOGO_Y + 5);
    char_y_start = char_y = (LOGO_Y + 2 + FONT_HEIGHT) / FONT_HEIGHT;
}

int GRAPHIC_init(struct core_env *env) {
    multiboot_info_t *multiboot_header = &env->boot_info;
    u32               fb_addr = multiboot_header->framebuffer_addr & 0xFFFFFFFF;
    __GRAPHIC_init((void *)(fb_addr), multiboot_header->framebuffer_width,
                   multiboot_header->framebuffer_height,
                   multiboot_header->framebuffer_pitch);
    return 0;
}

static Driver_Declaration driver_graphic = {.magic       = DRIVER_DC,
                                            .name        = "GRAPHIC",
                                            .major_ver   = 0,
                                            .minor_ver   = 1,
                                            .level       = 0,
                                            .init        = GRAPHIC_init,
                                            .initialized = FALSE};

ADD_DRIVER(driver_graphic);