//
//
//
//
//

#ifndef BARE_DEFINES_H
#define BARE_DEFINES_H

#include <stdint.h>
#include "board.h"

#define ALWAYS_INLINE __attribute__ ((always_inline)) static inline
#define ASM asm volatile

// *** REGISTER ACCESS ***
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
// ** END REG ACCESS **

#define WAIT asm volatile("")

#define BREAK(break_num) asm volatile ("bkpt "#break_num)


#endif
