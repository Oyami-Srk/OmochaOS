#ifndef __CONFIG_H__
#define __CONFIG_H__

#define PRINT_CLOCK 1

#if APIC
#define USE_APIC 1
#else
#define USE_8259A 1
#endif

#endif // __CONFIG_H__