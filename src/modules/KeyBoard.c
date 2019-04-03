#include "KeyBoard.h"
#include "KeyMap.h"
#include "Monitor.h"
#include "kernel/type.h"
#include "kernel/asm.h"
#include "modules/modules.h"

Circular_BufferB kbd_buffer;

uint shift_l, shift_r, alt_l, alt_r, ctrl_l, ctrl_r, caps_lock, num_lock,
    scr_lock;
uint E0, col;

void kbd_wait() {
  unsigned char stat;
  do {
    stat = inb(0x64);
  } while (stat & 0x02); // wait for buffer cleanned
}

void kbd_ack() {
  unsigned char data;
  do {
    data = inb(0x60);
  } while (data != 0xFA);
}

void set_leds() {
  unsigned char leds = (caps_lock << 2) | (num_lock << 1) | scr_lock;

  kbd_wait();
  outb(0x60, 0xED);
  kbd_ack();

  kbd_wait();
  outb(0x60, leds);
  kbd_ack();
}

uint kbd_read() {
  unsigned char code;
  int make;
  uint key = 0;
  uint *keyrow = 0;
  if (kbd_buffer.count > 0) {
    E0 = 0;
    code = read_circular_buffer_b(&kbd_buffer);
    if (code == 0xE1) {
      unsigned char pausebrk_scode[] = {0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5};
      int is_pausebreak = 1;
      for (uint i = 0; i < 6; i++)
        if (read_circular_buffer_b(&kbd_buffer) != pausebrk_scode[i]) {
          is_pausebreak = 0;
          break;
        }
      if (is_pausebreak)
        key = PAUSEBREAK;
    } else if (code == 0xE0) {
      code = read_circular_buffer_b(&kbd_buffer);

      // PrintScreen
      if (code == 0x2A) {
        if (read_circular_buffer_b(&kbd_buffer) == 0xE0)
          if (read_circular_buffer_b(&kbd_buffer) == 0x37) {
            key = PRINTSCREEN;
            make = 1;
          }
      }

      // PrintScreen - Release
      if (code == 0xB7)
        if (read_circular_buffer_b(&kbd_buffer) == 0xE0)
          if (read_circular_buffer_b(&kbd_buffer) == 0xAA) {
            key = PRINTSCREEN;
            make = 0;
          }

      if (key == 0)
        E0 = 1;
    }

    if ((key != PAUSEBREAK) && (key != PRINTSCREEN)) {
      make = (code & FLAG_BREAK ? 0 : 1);
      keyrow = &keymap[((code & 0x7F) * MAP_COLS)];
      col = 0;

      int caps = shift_l || shift_r;
      if (caps_lock)
        if ((keyrow[0] >= 'a') && (keyrow[0] <= 'z'))
          caps = !caps;
      if (caps)
        col = 1;
      if (E0)
        col = 2;
      key = keyrow[col];
      switch (key) {
      case SHIFT_L:
        shift_l = make;
        break;
      case SHIFT_R:
        shift_r = make;
        break;
      case ALT_L:
        alt_l = make;
        break;
      case ALT_R:
        alt_r = make;
        break;
      case CTRL_L:
        ctrl_l = make;
        break;
      case CTRL_R:
        ctrl_r = make;
        break;
      case CAPS_LOCK:
        if (make) {
          caps_lock = !caps_lock;
          set_leds();
        }
        break;
      case NUM_LOCK:
        if (make) {
          num_lock = !caps_lock;
          set_leds();
        }
        break;
      case SCROLL_LOCK:
        if (make) {
          scr_lock = !scr_lock;
          set_leds();
        }
        break;
      default:
        break;
      }
    }
    if (make) {
      key |= shift_l ? FLAG_SHIFT_L : 0;
      key |= shift_r ? FLAG_SHIFT_R : 0;
      key |= alt_l ? FLAG_ALT_L : 0;
      key |= alt_r ? FLAG_ALT_R : 0;
      key |= ctrl_l ? FLAG_CTRL_L : 0;
      key |= ctrl_r ? FLAG_CTRL_R : 0;
      return key;
    }
    return 0;
  }
  return 0;
}

void initialize_kbd() {
  kbd_buffer.tail = kbd_buffer.head = kbd_buffer.buf;
  kbd_buffer.count = 0;
  shift_l = shift_r = alt_l = alt_r = ctrl_l = ctrl_r = caps_lock = num_lock =
      scr_lock = 0;
  num_lock = 1;
  // set leds
  set_leds();
  if (register_to(KBD_IRQ, INT_HANDLE_METHOD_CIRCULAR_BUFFER,
                  (uint)&kbd_buffer) != 0)
    kprintf("Cannot get assigned for KBD interrupt!\n");
}
