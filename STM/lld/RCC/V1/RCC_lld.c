//
//
//
//
//


#include "RCC_lld.h"

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t RCC_Enable(struct RCCxo *RCCo)
{
	volatile uint32_t * const clockreg = 
		(void *)((uint32_t)RCC + RCCo->clockregoffset);
	//get correct clock reg address

	uint32_t setbit = 1 << RCCo->clockbitoffset;
	//get correct bit

	uint32_t prevstate = *clockreg & setbit;
	//get previous state

	*clockreg |= setbit;
	//enable clock bit
	
	return prevstate;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RCC_Disable(struct RCCxo *RCCo)
{
	volatile uint32_t * const clockreg = 
		(void *)((uint32_t)RCC + RCCo->clockregoffset);
	//get correct clock reg address

	uint32_t resetbit = ~(1 << RCCo->clockbitoffset);
	//get correct bit

	*clockreg &= resetbit;
	//disable clock bit
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RCC_Reset(struct RCCxo *RCCo)
{
	uint32_t *resetreg = 
		(void *)((uint32_t)RCC + RCCo->clockregoffset - 0x20);
	//get correct clock reg address

	uint32_t bit = 1 << RCCo->clockbitoffset;
	//get correct bit

	*resetreg |= bit;
	//enable reset bit

	for(int i = 10; i != 0; i--)
	{
		asm volatile("");
	}
	//wait for 35 ish clock cycles to let things reset before turning proceding.

	*resetreg &= ~bit;
	//disable reset bit
}




