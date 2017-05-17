//
//
//
//
//



#ifndef NVIC_H
#define NVIC_H

#include "board.h"

void NvicSetInterruptPriority(IRQn_Type IRQn, int PRIO);
void NvicSetSystemInterruptPriority(IRQn_Type IRQn, int PRIO);
uint32_t NvicGetInterruptPriority(IRQn_Type IRQn);
uint32_t NvicGetSystemInterruptPriority(IRQn_Type IRQn);

void NvicEnableInterrupt(IRQn_Type IRQn);

void NvicDisableInterrupt(IRQn_Type IRQn);





#endif
