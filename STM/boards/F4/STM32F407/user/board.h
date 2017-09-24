//
//
//

#include "stm32f4xx.h"
#include "bare_defines.h"

#define HSI_SPEED 16000000
#define VCO_MAX 432000000
#define USB_SPEED 48000000
#define CPU_MAX 168000000
#define AHB_MAX 168000000
#define APB1_MAX 42000000
#define APB2_MAX 84000000
//board constants

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
//#define TIMER_INIT_FUNCTION SysTickEnable
//#define TIMER_UPDATE_FUNCTION SysTickUpdate
//#define TIMER_ISR SysTick_Handler
