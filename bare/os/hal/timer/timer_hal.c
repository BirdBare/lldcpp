//
//
//
//
//



#include "timer_hal.h"

uint32_t TimerConfigTimer(
	struct TimerObject *timer_object,
	struct TimerConfig *timer_config)
{
	return LldTimerConfigTimer(timer_object, timer_config);
}


uint32_t TimerStartTimerPolled(struct TimerObject *timer_object,
	uint32_t milliseconds)
{
	return LldTimerStartTimerPolled(timer_object, milliseconds);
}

uint32_t TimerStartTimerInterrupt(struct TimerObject *timer_object,
	uint32_t milliseconds)
{
	return LldTimerStartTimerInterrupt(timer_object, milliseconds);
}


