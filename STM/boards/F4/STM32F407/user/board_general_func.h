//
//
//
//
//

#include "stm32f4xx.h"
#include "clock_lld.h"
#include "flash_lld.h"
#include "systick_lld.h"

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
	SysTickUpdate(); \
	SysTickEnable(); \
} while(0)
