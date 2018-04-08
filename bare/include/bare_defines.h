//
//
//
//
//

#ifndef BARE_DEFINES_H
#define BARE_DEFINES_H

#include <stdint.h>

#define NOP WAIT
#define WAIT asm volatile("nop")

#define BREAK(break_num) asm volatile ("bkpt "#break_num)


#endif
