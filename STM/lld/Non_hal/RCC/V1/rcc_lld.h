//
//
//
//
//



#ifndef RCC_H
#define RCC_H

#include "board.h"

//Object struct of the Reset and clock control. Every object who contains
// a clock must contain this in the structure first. This will allow us to
// enable the clock from the object.

struct RccObject
{
	const uint8_t reg_offset;	//address offset for the clock register.

	//LSB
	const uint8_t bit_offset:5; //bitshift offset for the clock enable bit.

	const uint8_t peripheral_bus:3; //peripheral bus. AHB,APB1, or APB2
	//MSB												 //used in clock lld

};


#define RCC_CLOCK_ENABLED 1

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
