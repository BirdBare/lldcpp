//
//
//
//
//



#ifndef CLOCK_HAL_H
#define CLOCK_HAL_H

#include "clock_lld.h"
#include "bareos.h"

uint32_t ClockResetConfig(void);
uint32_t ClockConfig(const struct ClockConfig * const clock_config);
static inline uint32_t ClockGetCpuSpeed(void)
{
	return LldClockGetCpuSpeed();
}


#endif
