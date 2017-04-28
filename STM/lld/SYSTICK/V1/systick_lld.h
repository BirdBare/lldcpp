//
//
//
//
//



//SYSTICK WILL ALWAYS INTERRUPT EVERY MILLISECOND.
// If you want a different tick rate. Use the Timers


#ifndef SYSTICK_H
#define SYSTICK_H

#include "board.h"
#include "bare_defines.h"
#include "clock_lld.h"

extern volatile uint32_t systick_milli;

#define SysTickGetMilli(void) systick_milli


#define SYSTICK_CTRL_ENABLE 0b1
#define SYSTICK_CTRL_TICKINT 0b10

ALWAYS_INLINE void SysTickUpdate(void)
{
	SysTick->LOAD = (ClockGetAhbSpeed() >> 3) / 1000;
	//Clock speed divided by 8000 equals ticks per millisecond
}

ALWAYS_INLINE void SysTickEnable(void)
{
	SysTick->CTRL = SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_TICKINT;
	//Enable Systick with clock AHB/8. Interrupt also Enabled
}

ALWAYS_INLINE void SysTickDisable(void)
{
	SysTick->CTRL = 0;
}

void SysTick_Handler(void);

void SysTickDelayMilli(uint32_t milli);


#endif
