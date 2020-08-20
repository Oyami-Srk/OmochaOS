#ifndef __DRIVER_MISC_H__
#define __DRIVER_MISC_H__

#include <core/config.h>
#include <generic/asm.h>

#define TIMER_USE_8253

#ifdef TIMER_USE_8253
#define FREQ_8253 1193180

static inline void init_8253() {
#define I8253_CTR_PORT  0x43
#define I8253_CNL0      0x40
#define I8253_CNL0_DATA (FREQ_8253 / BEATS_RATE)
    outb(I8253_CTR_PORT, 0x34);
    outb(I8253_CNL0, (unsigned short)((I8253_CNL0_DATA)&0xFF));
    outb(I8253_CNL0, (unsigned short)((I8253_CNL0_DATA >> 8) & 0xFF));
}

static inline void disable_8253() {
#define I8253_CTR_PORT 0x43
#define I8253_CNL0     0x40
    outb(I8253_CTR_PORT, 0x32); // one-shot
    outb(I8253_CNL0, 0);
    outb(I8253_CNL0, 0);
}
#endif
#endif // __MISC_H__
