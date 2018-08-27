//
//
//
//
//



#ifndef CLOCK_HPP
#define CLOCK_HPP

#include "board.h"

#ifndef HSI_SPEED
	#error "HSI_SPEED MISSING. V2 Clock.h" 
#endif
#ifndef USB_SPEED
	#error "USB_SPEED MISSING. V2 Clock.h"
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
};


//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t LldClockInit(void)
{
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t LldClockDeinit(void)
{
	return 0;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t LldClockConfig(
const struct ClockConfig * const clock_config);


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t LldClockResetConfig(void);




#endif
