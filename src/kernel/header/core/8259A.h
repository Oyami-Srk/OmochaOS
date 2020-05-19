#ifndef __8259A_H__
#define __8259A_H__

#include "core/environment.h"
#include "generic/typedefs.h"

void init_8259A(struct core_env *env);
#define init_inthw init_8259A
void end_interrupt(uint i);
void enable_interrupt(uint i);
void disable_interrupt(uint i);

#endif