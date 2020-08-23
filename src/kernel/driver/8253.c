#include <driver/driver.h>
#include <generic/asm.h>

#define FREQ_8253 1193180

int init_8253(struct core_env *env) {
#define I8253_CTR_PORT  0x43
#define I8253_CNL0      0x40
#define I8253_CNL0_DATA (FREQ_8253 / BEATS_RATE)
    outb(I8253_CTR_PORT, 0x34);
    outb(I8253_CNL0, (unsigned short)((I8253_CNL0_DATA)&0xFF));
    outb(I8253_CNL0, (unsigned short)((I8253_CNL0_DATA >> 8) & 0xFF));
    return 0;
}

void disable_8253() {
#define I8253_CTR_PORT 0x43
#define I8253_CNL0     0x40
    outb(I8253_CTR_PORT, 0x32); // one-shot
    outb(I8253_CNL0, 0);
    outb(I8253_CNL0, 0);
}

static Driver_Declaration driver_8259A = {.magic       = DRIVER_DC,
                                          .name        = "8253",
                                          .major_ver   = 0,
                                          .minor_ver   = 1,
                                          .level       = -1,
                                          .init        = init_8253,
                                          .initialized = FALSE};

ADD_DRIVER(driver_8259A);