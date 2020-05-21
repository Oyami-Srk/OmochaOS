#ifndef __CONFIG_H__
#define __CONFIG_H__

#define PRINT_CLOCK 0

// beats rate in hz, 100 beats a second
#define BEATS_RATE 100
#define BEATS_MS   (1000 / BEATS_RATE)

#if APIC
#define USE_APIC 1
#else
#define USE_8259A 1
#endif

#endif // __CONFIG_H__