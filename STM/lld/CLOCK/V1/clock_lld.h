//
//
//
//
//



#ifndef CLOCK_H
#define CLOCK_H

#include "board.h"
#include "rcc_lld.h"

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

#ifndef HSI_SPEED
	#error "HSI_SPEED MISSING. V2 Clock.h" 
#endif
#ifndef VCO_MAX
	#error "VCO_MAX MISSING. V2 Clock.h"
#endif
#ifndef VCO_MIN
	#error "VCO_MIN MISSING. V2 Clock.h"
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

struct ClockConfig
{
	uint32_t cpu_speed; //speed of the microcontroller in hz
	uint32_t ahb_speed; //speed of the hardware bus in hz
	uint32_t apb1_speed; //speed of low speed peripheral bus in hz
	uint32_t apb2_speed; //speed of high speed peripheral bus in hz

	uint32_t hse_speed; //speed of external clock in hz. if none is used. keep zero
	uint32_t usb_sdio_rng_speed; //speed of pll that drives usb, sd, and rand gen
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
uint32_t ClockResetConfig(void);


//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t ClockGetSpeed(uint32_t bus)
{
	return CLOCK_SPEED[bus];
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t ClockGetPeripheralSpeed(
	const struct RccObject * const rcc_object)
{
	return ClockGetSpeed(rcc_object->peripheral_bus);
}

//******************************************************************************
ALWAYS_INLINE uint32_t ClockGetPeripheralPrescaler(
	const struct RccObject * const rcc_object)
{
	return CLOCK_PRESCALER[rcc_object->peripheral_bus];
}




#endif
