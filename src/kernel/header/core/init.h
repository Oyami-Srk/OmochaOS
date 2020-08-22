#ifndef __INIT_H__
#define __INIT_H__

#include <core/environment.h>
#include <driver/driver.h>

void core_init_interrupt(struct core_env *env);
void core_init_protect(struct core_env *env);
void core_init_proc(struct core_env *env);
void core_init_memory(struct core_env *env);
void core_init_driver(struct core_env *env, Driver_Declaration *drivers);

#endif // __INIT_H__
