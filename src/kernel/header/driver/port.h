#ifndef __PORT_H__
#define __PORT_H__

int  init_port(void);
void write_serial(char a);
void printf_serial(const char *fmt, ...);

#endif // __PORT_H__