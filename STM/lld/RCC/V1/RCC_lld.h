//
//
//
//
//



#ifndef STM32F767_RCC_H
#define STM32F767_RCC_H

#include "BARE_STM32F767.h"
#include "board.h"

#define RCC_CLOCK_ENABLED 1

//Object struct of the Reset and clock control. Every object who contains
// a clock must contain this in the structure first. This will allow us to
// enable the clock from the object.

struct RCCxo
{
	const uint8_t clockregoffset;	//address offset for the clock register.

	const uint8_t clockbitoffset; //bitshift offset for the clock enable bit.
};



//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t RCC_Enable(struct RCCxo *RCCo);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RCC_Disable(struct RCCxo *RCCo);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RCC_Reset(struct RCCxo *RCCo);



#endif
