#ifndef __INIT_H__
#define __INIT_H__

#include "core/environment.h"
void core_init_interrupt(struct core_env *env);
void core_init_gdt(struct core_env *env);
void core_init_proc(struct core_env *env);
#endif // __INIT_H__
