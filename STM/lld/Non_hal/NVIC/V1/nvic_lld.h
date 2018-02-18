//
//
//
//
//



#ifndef NVIC_H
#define NVIC_H

#include "board.h"

struct InterruptObject
{
	const uint8_t irq_number; //IRQn_Type from stm32 driver definitions header
	
	const uint8_t num_irq; //number of interrupts to enable in for the peripheral
};

void NvicSetInterruptPriority(IRQn_Type IRQn, int PRIO);
void NvicSetSystemInterruptPriority(IRQn_Type IRQn, int PRIO);
uint32_t NvicGetInterruptPriority(IRQn_Type IRQn);
uint32_t NvicGetSystemInterruptPriority(IRQn_Type IRQn);

void NvicEnableInterrupt(IRQn_Type IRQn);

void NvicDisableInterrupt(IRQn_Type IRQn);





#endif
