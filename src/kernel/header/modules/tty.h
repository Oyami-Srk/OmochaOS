#ifndef __MODULE_TTY_H__
#define __MODULE_TTY_H__

#define TTY_OPEN 1
#define TTY_READ 2
#define TTY_WRITE 3
#define TTY_CLOSE 4

int printf(const char *fmt, ...);

#include "modules/systask.h"

// type = 0: tty do output
// type = 1: tty only do write and read
inline int tty_open(int type){

}

inline int getkey(){

}

inline int readline(char *buf, unsigned int buf_size){

}



#endif // __MODULE_TTY_H__
