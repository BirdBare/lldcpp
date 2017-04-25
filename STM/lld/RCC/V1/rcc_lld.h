//
//
//
//
//



#ifndef STM32F767_RCC_H
#define STM32F767_RCC_H

#include"BARE_STM32F767.h"
#include <stdint.h>
#include "board.h"

#define RCC_CLOCK_ENABLED 1

//Object struct of the Reset and clock control. Every object who contains
// a clock must contain this in the structure first. This will allow us to
// enable the clock from the object.

struct RccObject
{
	const uint8_t reg_offset;	//address offset for the clock register.

	const uint8_t bit_offset; //bitshift offset for the clock enable bit.
};



//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t RccEnableClock(const struct RccObject * const rcc_object);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccDisableClock(const struct RccObject * const rcc_object);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccResetPeripheral(const struct RccObject * const rcc_object);





#endif
