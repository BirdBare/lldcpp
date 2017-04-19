//
//
//
//
//



#ifndef STM32F767_SYSTICK_H
#define STM32F767_SYSTICK_H

#include "board.h"

#define SysTick_EnableTimer(void) \
do \
{ \
	SysTick_Config(0b111, _FCPU / 1000); \
	SysTick_Enable(); \
} while (0);



extern volatile uint32_t SysTick_Ticks;

#define SYSTICK_CONFIG_ENABLED 1

int SysTick_Config(int CTRL, uint32_t TicksPerInterrupt);
int SysTick_ResetConfig(void);
void SysTick_Enable(void);
void SysTick_Disable(void);

int SysTick_MilliSec(void);

void DelayMilli(uint32_t Milli);

void SysTick_Handler(void);


#endif
