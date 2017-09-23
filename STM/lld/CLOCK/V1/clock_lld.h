//
//
//
//
//



#ifndef CLOCK_H
#define CLOCK_H

#include "board.h"
#include "rcc_lld.h"

extern volatile uint32_t CLOCK_SPEED[4];
//Clock Speeds for CPU, AHB, APB1, APB2 clocks in that order.
//AHB max is cpu max, APB1 max is AHB max / 4. APB2 max is AHB max / 2.
#define CPU 0
#define AHB 1
#define APB1 2
#define APB2 3

#ifndef HSI_SPEED
	#error "HSI_SPEED MISSING. V2 Clock.h" 
#endif
#ifndef VCO_MAX
	#error "VCO_MAX MISSING. V2 Clock.h"
#endif
#ifndef USB_SPEED
	#error "USB_SPEED MISSING. V2 Clock.h"
#endif
#ifndef CPU_MAX
	#error "CPU_MAX MISSING. V2 Clock.h"
#endif
#ifndef AHB_MAX
	#error "AHB_MAX MISSING. V2 Clock.h"
#endif
#ifndef APB1_MAX
	#error "APB1_MAX MISSING. V2 Clock.h"
#endif
#ifndef APB2_MAX 
	#error "APB2_MAX MISSING. V2 Clock.h"
#endif
//Board CONSTANTS

static uint32_t LogBase2(uint32_t number)
{
	uint32_t count = 0;
	while(number > 1)
	{
		number >>= 1;
		count++;
	}

	return count;
}


struct ClockConfig
{
	uint32_t cpu_speed; //speed of the microcontroller
	uint32_t ahb_speed; //speed of the hardware bus
	uint32_t apb1_speed; //speed of low speed peripheral bus
	uint32_t apb2_speed; //speed of high speed peripheral bus

	uint32_t hse_speed; //speed of external clock in MHz. if none is used. keep zero
};

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t ClockConfig(
const struct ClockConfig * const clock_config);



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t ClockGetPeripheralSpeed(
	const struct RccObject * const rcc_object)
{
	return CLOCK_SPEED[rcc_object->clock_index];
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t ClockGetCpuSpeed(void)
{
	return CLOCK_SPEED[0];
}

ALWAYS_INLINE uint32_t ClockGetAhbSpeed(void)
{
	return CLOCK_SPEED[1];
}

#endif
