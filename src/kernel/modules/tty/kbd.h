#ifndef __MODULE_KBD_H__
#define __MODULE_KBD_H__

#include "keycode.h"

#define KBD_IRQ 1

#define KBD_BUFFER_SIZE 32

#define MSG_KBD_GETKEY 1
#define MSG_KBD_READ   1

void init_kbd();
// fill data to buffer
void         kbd_write(unsigned char data);
unsigned int kbd_read();
unsigned int kbd_peak();
unsigned int kbd_buffer_used();
int          kbd_code_end();

#endif // __MODULE_KBD_H__
