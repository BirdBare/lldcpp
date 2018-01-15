//
//
//
//
//



#ifndef FPU_H
#define FPU_H

#include "board.h"

void FpuInit(void)
{
	SCB->CPACR |= 0b1111 << 20;
	FPI->FPCCR |= 0b11 << 30;

	asm volatile("DSB");
	asm volatile("ISB");
}


#endif
