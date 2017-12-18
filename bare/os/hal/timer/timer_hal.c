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
	uint32_t counts)
{
	return LldTimerStartTimerPolled(timer_object, counts);
}

uint32_t TimerStartTimerInterrupt(struct TimerObject *timer_object,
	uint32_t counts)
{
	return LldTimerStartTimerInterrupt(timer_object, counts);
}


