//
//
//
//
//



#ifndef RCC_H
#define RCC_H

#include "board.h"



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


struct RccLld
{
	volatile uint32_t * const enable_register;	//address offset for the clock register.

	//LSB
	const uint32_t peripheral_bit; //bitshift offset for the clock enable bit.

	volatile uint32_t * const reset_register;

	const enum RCC_PERIPHERAL_BUS peripheral_bus; //peripheral bus. AHB,APB1, or APB2
	//MSB												 //used in clock lld
};

static inline bool_t RccLldInit(struct RccLld *rcc)
{
	*rcc->enable_register |= rcc->peripheral_bit;

	return true;
}

static inline bool_t RccLldIsInit(struct RccLld *rcc)
{
	return *rcc->enable_register & rcc->peripheral_bit;
}

static inline bool_t RccLldDeinit(struct RccLld *rcc)
{
	*rcc->enable_register &= rcc->peripheral_bit;

	return true;
}

static inline bool_t RccLldReset(struct RccLld *rcc)
{

	if(!RccLldIsInit(rcc) || rcc->reset_register == 0)
	{
	 return false;
	}
		*rcc->reset_register |= rcc->peripheral_bit;
		*rcc->reset_register &= rcc->peripheral_bit;

	return true;
}

static inline uint32_t RccLldGetPeripheralClockSpeed(struct RccLld *rcc)
{
	return CLOCK_SPEED[rcc->peripheral_bus];
}

static inline uint32_t RccLldGetPeripheralClockPrescaler(struct RccLld *rcc)
{
	return CLOCK_PRESCALER[rcc->peripheral_bus];
}

static inline uint32_t RccLldClockSpeed(void)
{
	return CLOCK_SPEED[RCC_PERIPHERAL_BUS_CPU];
}

#define RCC_CLOCK_ENABLED 1

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t RccLldEnableClock(const struct RccLld * const rcc);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccLldDisableClock(const struct RccLld * const rcc);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccLldResetPeripheral(const struct RccLld * const rcc);

//******************************************************************************
//  
//                     
//  
//******************************************************************************
static uint32_t RccLldGetSpeed(enum RCC_PERIPHERAL_BUS bus)
{
	return CLOCK_SPEED[bus];
}
#define LldClockGetCpuSpeed(void) LldClockGetSpeed(CPU);

//******************************************************************************
//  
//                     
//  
//******************************************************************************
static uint32_t RccLldGetPeripheralSpeed(
	const struct RccLld * const rcc)
{
	return RccLldGetSpeed(rcc->peripheral_bus);
}

//******************************************************************************
static uint32_t RccLldGetPeripheralPrescaler(
	const struct RccLld * const rcc)
{
	return CLOCK_PRESCALER[rcc->peripheral_bus];
}





#endif


