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


enum RCC_PERIPHERAL_BUS
{
  RCC_PERIPHERAL_BUS_APB1 = 0,
  RCC_PERIPHERAL_BUS_APB2 = 1,
  RCC_PERIPHERAL_BUS_AHB = 2,
  RCC_PERIPHERAL_BUS_CPU = 3,
  RCC_PERIPHERAL_BUS_USB = 4
};

extern volatile uint32_t CLOCK_SPEED[5];
//Clock Speeds for APB1, APB2, AHB, CPU, USB clocks in that order in Mhz.
//AHB max is cpu max, APB1 max is AHB max / 4. APB2 max is AHB max / 2.
extern volatile uint16_t CLOCK_PRESCALER[2];
//Clock Prescalers for APB1, APB2




//Hal struct of the Reset and clock control. Every object who contains
// a clock must contain this in the structure first. This will allow us to
// enable the clock from the object.

struct RccHal
{
	volatile uint32_t * const enable_register;	//address offset for the clock register.

	//LSB
	const uint32_t peripheral_bit; //bitshift offset for the clock enable bit.

	volatile uint32_t * const reset_register;

	const enum RCC_PERIPHERAL_BUS peripheral_bus; //peripheral bus. AHB,APB1, or APB2
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
static uint32_t RccGetSpeed(enum RCC_PERIPHERAL_BUS bus)
{
	return CLOCK_SPEED[bus];
}
#define LldClockGetCpuSpeed(void) LldClockGetSpeed(CPU);

//******************************************************************************
//  
//                     
//  
//******************************************************************************
static uint32_t RccGetPeripheralSpeed(
	const struct RccHal * const rcc_object)
{
	return RccGetSpeed(rcc_object->peripheral_bus);
}

//******************************************************************************
static uint32_t RccGetPeripheralPrescaler(
	const struct RccHal * const rcc_object)
{
	return CLOCK_PRESCALER[rcc_object->peripheral_bus];
}



#ifdef __cplusplus
}
#endif



#endif


