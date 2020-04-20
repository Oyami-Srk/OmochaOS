/* original font bitmap from
 * https://courses.cs.washington.edu/courses/cse457/98a/tech/OpenGL/font.c
 * convert by Shiroko<hhx.xxm@gmail.com>
 * All rights reserved by original site.
 * (c) Copyright 1993, Silicon Graphics, Inc.
 */

#define FONT_WIDTH  8
#define FONT_HEIGHT 13
#define FONT_OFFSET 32
#define FONT_GAP_X  1

static const unsigned char font_block[13] = {0x7E, 0x42, 0x42, 0x42, 0x42,
                                             0x42, 0x42, 0x42, 0x42, 0x42,
                                             0x42, 0x7E, 0x00};

static const unsigned char font[95][13] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00,
     0x00},
    {0x6c, 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00},
    {0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00,
     0x00},
    {0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18, 0x00,
     0x00},
    {0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70, 0x00,
     0x00},
    {0x1c, 0x36, 0x33, 0x33, 0x1b, 0x0e, 0x0e, 0x1b, 0xf3, 0x63, 0xfe, 0x00,
     0x00},
    {0x70, 0x30, 0x38, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00},
    {0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00,
     0x00},
    {0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00,
     0x00},
    {0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00, 0x00,
     0x00},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x18, 0x0c, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00,
     0x00},
    {0xc0, 0xc0, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06,
     0x00},
    {0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c, 0x00,
     0x00},
    {0x18, 0x1c, 0x1e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00,
     0x00},
    {0x7e, 0xe7, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0xff, 0x00,
     0x00},
    {0x7e, 0xe7, 0xc0, 0xc0, 0xe0, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e, 0x00,
     0x00},
    {0x30, 0x38, 0x3c, 0x36, 0x33, 0xff, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00,
     0x00},
    {0xff, 0x03, 0x03, 0x03, 0x03, 0x7f, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e, 0x00,
     0x00},
    {0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe3, 0xc3, 0xc3, 0xe7, 0x7e, 0x00,
     0x00},
    {0xff, 0xc0, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x00,
     0x00},
    {0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0x00,
     0x00},
    {0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x18, 0x0c, 0x00,
     0x00},
    {0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
     0x00},
    {0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x00,
     0x00},
    {0x7e, 0xc3, 0xc3, 0xc0, 0x60, 0x30, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x7e, 0xc3, 0xbb, 0xcb, 0xdb, 0xf3, 0x06, 0xfc, 0x00,
     0x00},
    {0x18, 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0x00,
     0x00},
    {0x7f, 0xe3, 0xc3, 0xc3, 0xe3, 0x7f, 0xe3, 0xc3, 0xc3, 0xe3, 0x7f, 0x00,
     0x00},
    {0x7e, 0xe7, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xe7, 0x7e, 0x00,
     0x00},
    {0x3f, 0x73, 0xe3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe3, 0x73, 0x3f, 0x00,
     0x00},
    {0xff, 0x03, 0x03, 0x03, 0x03, 0x3f, 0x03, 0x03, 0x03, 0x03, 0xff, 0x00,
     0x00},
    {0xff, 0x03, 0x03, 0x03, 0x3f, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00,
     0x00},
    {0x7e, 0xe7, 0x03, 0x03, 0x03, 0x03, 0xf3, 0xc3, 0xc3, 0xe7, 0x7e, 0x00,
     0x00},
    {0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00,
     0x00},
    {0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00,
     0x00},
    {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x63, 0x77, 0x3e, 0x00,
     0x00},
    {0xc3, 0x63, 0x33, 0x1b, 0x0f, 0x07, 0x0f, 0x1b, 0x33, 0x63, 0xc3, 0x00,
     0x00},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xff, 0x00,
     0x00},
    {0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00,
     0x00},
    {0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3, 0x00,
     0x00},
    {0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e, 0x00,
     0x00},
    {0x7f, 0xe3, 0xc3, 0xc3, 0xe3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00,
     0x00},
    {0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xfb, 0x76, 0xfc, 0x00,
     0x00},
    {0x7f, 0xe3, 0xc3, 0xc3, 0xe3, 0x7f, 0x0f, 0x1b, 0x33, 0x63, 0xc3, 0x00,
     0x00},
    {0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e, 0x00,
     0x00},
    {0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,
     0x00},
    {0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e, 0x00,
     0x00},
    {0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x00,
     0x00},
    {0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xdb, 0xff, 0xff, 0xe7, 0xc3, 0x00,
     0x00},
    {0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0x00,
     0x00},
    {0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,
     0x00},
    {0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff, 0x00,
     0x00},
    {0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0x00,
     0x00},
    {0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60, 0xc0, 0xc0,
     0x00},
    {0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00,
     0x00},
    {0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
     0xff},
    {0x0e, 0x0c, 0x1c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xfe, 0xc3, 0xc3, 0xfe, 0x00,
     0x00},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0x03, 0x03, 0x03, 0xc3, 0x7e, 0x00,
     0x00},
    {0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0xfe, 0x00,
     0x00},
    {0x78, 0xcc, 0x0c, 0x0c, 0x0c, 0x3f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc3,
     0x7e},
    {0x03, 0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00,
     0x00},
    {0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,
     0x00},
    {0x00, 0x30, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x36,
     0x1c},
    {0x03, 0x03, 0x03, 0x03, 0x63, 0x33, 0x1b, 0x0f, 0x1f, 0x33, 0x63, 0x00,
     0x00},
    {0x1e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x7f, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x3f, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x3e, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3e, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03,
     0x03},
    {0x00, 0x00, 0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0,
     0xc0},
    {0x00, 0x00, 0x00, 0x00, 0x7f, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00,
     0x00},
    {0x00, 0x0c, 0x0c, 0x0c, 0x3f, 0x0c, 0x0c, 0x0c, 0x0c, 0x6c, 0x38, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7e, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0xc3, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xe7, 0xc3, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x18, 0x0c, 0x06, 0x06,
     0x03},
    {0x00, 0x00, 0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00,
     0x00},
    {0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0, 0x00,
     0x00},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
     0x18},
    {0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f, 0x00,
     0x00},
    {0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00},
};
