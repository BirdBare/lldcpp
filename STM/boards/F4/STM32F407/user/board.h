//
//
//
//
//

#include "stm32f4xx.h"

//Required for bare_timer.h
#define TIMER_INIT_FUNCTION SysTickEnable
#define TIMER_UPDATE_FUNCTION SysTickUpdate
#define TIMER_ISR SysTick_Handler
