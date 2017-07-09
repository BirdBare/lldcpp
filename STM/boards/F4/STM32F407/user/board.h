//
//
//
//
//

#include "stm32f4xx.h"
#include "bare_defines.h"

#define STARTUP(void) \
do \
{ \
	FlashEnableArt(&FLASH_OBJECT); \
	struct FlashConfig flash_config = {5}; \
	FlashConfig(&FLASH_OBJECT,&flash_config); \
	\
	struct ClockConfig clock_config = CLOCKCONFIG_168MHZ; \
	ClockConfig(&clock_config); \
	\
} while(0)


//Required for bare_timer.h
#define TIMER_INIT_FUNCTION SysTickEnable
#define TIMER_UPDATE_FUNCTION SysTickUpdate
#define TIMER_ISR SysTick_Handler
