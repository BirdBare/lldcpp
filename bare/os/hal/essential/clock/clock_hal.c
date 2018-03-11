//
//
//
//
//



#include "clock_hal.h"

uint32_t ClockConfig(const struct ClockConfig * const clock_config)
{
	BareOSDisableInterrupts();

	uint32_t ret = LldClockConfig(clock_config);

	if(ret == 0)
	{
		BareOSTimerUpdateTimer();
	}

	BareOSEnableInterrupts();

	return ret;
}

uint32_t ClockResetConfig(void)
{
	BareOSDisableInterrupts();

	uint32_t ret = LldClockResetConfig();

	BareOSEnableInterrupts();

	return ret;
}

