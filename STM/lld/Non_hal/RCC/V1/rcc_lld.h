//
//
//
//
//



#ifndef RCC_H
#define RCC_H

#include "board.h"


#ifdef __cplusplus
extern "C" {
#endif



extern volatile uint32_t CLOCK_SPEED[5];
//Clock Speeds for APB1, APB2, AHB, CPU, USB clocks in that order in Mhz.
//AHB max is cpu max, APB1 max is AHB max / 4. APB2 max is AHB max / 2.
extern volatile uint16_t CLOCK_PRESCALER[2];
//Clock Prescalers for APB1, APB2
#define APB1 0
#define APB2 1
#define AHB 2
#define CPU 3
#define USB 4


//Hal struct of the Reset and clock control. Every object who contains
// a clock must contain this in the structure first. This will allow us to
// enable the clock from the object.

struct RccHal
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
uint32_t RccEnableClock(const struct RccHal * const rcc_object);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccDisableClock(const struct RccHal * const rcc_object);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccResetPeripheral(const struct RccHal * const rcc_object);

//******************************************************************************
//  
//                     
//  
//******************************************************************************
static uint32_t LldClockGetSpeed(uint32_t bus)
{
	return CLOCK_SPEED[bus];
}
#define LldClockGetCpuSpeed(void) LldClockGetSpeed(CPU);

//******************************************************************************
//  
//                     
//  
//******************************************************************************
static uint32_t LldClockGetPeripheralSpeed(
	const struct RccHal * const rcc_object)
{
	return LldClockGetSpeed(rcc_object->peripheral_bus);
}

//******************************************************************************
static uint32_t LldClockGetPeripheralPrescaler(
	const struct RccHal * const rcc_object)
{
	return CLOCK_PRESCALER[rcc_object->peripheral_bus];
}



#ifdef __cplusplus
}
#endif



#endif


