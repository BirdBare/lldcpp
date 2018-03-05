//
//
//
//
//



#ifndef CLOCK_HAL_H
#define CLOCK_HAL_H

#include "clock_lld.h"
#include "bareos.h"

static inline uint32_t ClockInit(void)
{
	BareOSDisableInterrupts();

	uint32_t ret = LldClockInit();

	BareOSEnableInterrupts();

	return ret;
}

static inline uint32_t ClockDeinit(void)
{
	BareOSDisableInterrupts();

	uint32_t ret = LldClockDeinit();

	BareOSEnableInterrupts();
	
	return ret;
}

uint32_t ClockResetConfig(void);

uint32_t ClockConfig(const struct ClockConfig * const clock_config);

static inline uint32_t ClockGetCpuSpeed(void)
{
	BareOSDisableInterrupts();

	uint32_t ret = LldClockGetCpuSpeed();

	BareOSEnableInterrupts();

	return ret;
}


#endif
