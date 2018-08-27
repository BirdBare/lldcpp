//
//
//
//
//

#ifndef BARE_DEFINES_H
#define BARE_DEFINES_H

#include <stdint.h>

typedef bool error;
typedef bool status;

#define NOP WAIT
#define WAIT asm volatile("nop")
#define ASYNC_WAIT(void) WAIT

#define BREAK(break_num) asm volatile ("bkpt "#break_num)


#endif
