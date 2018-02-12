//
//
//
//
//



#include "clock_hal.h"

uint32_t ClockConfig(const struct ClockConfig * const clock_config)
{
	return LldClockConfig(clock_config);
}

uint32_t ClockResetConfig(void)
{
	return LldClockResetConfig();
}

