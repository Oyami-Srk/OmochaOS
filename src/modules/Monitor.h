#ifndef __OMOMOD_MONITOR_H__
#define __OMOMOD_MONITOR_H__

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define BLACK 0x0
#define BLUE  0x1
#define GREEN 0x2
#define RED   0x4
#define WHITE 0x7
#define FLASH 0x80
#define BRIGHT 0x08
#define COLOR(x,y) ((x << 4) | y)

typedef struct {
  unsigned int vm_start;
  unsigned int vm_end;
  unsigned int vm_disp;
  unsigned char *pCh;
}console;

#define MSG_MONITOR_INIT 0x01
#define MSG_MONITOR_FLUSH 0x02
#define MSG_MONITOR_PUTC 0x03
#define MSG_MONITOR_PUTS 0x04
#define MSG_MONITOR_SETCOLOR 0x05
#define MSG_MONITOR_CLRSCR 0x06
#define MSG_MONITOR_SWITCH 0x07


#endif
