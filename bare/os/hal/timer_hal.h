//
//
//
//
//



#ifndef TIMER_HAL_H
#define TIMER_HAL_H

#include "timer_lld.h"

// TimerConfig

// TimerSetTimer
// TimerGetTimer

// TimerEnablePwm
// TimerSetDutyCycle
// TimerGetDutyCycle

// TimerEnableInputCapture
// TimerGetInputCapture


static inline uint32_t TimerNanosecondsToTicks(
struct TimerObject *timer_object, 
	uint32_t nanoseconds)
{
	return LldTimerNanosecondsToTicks(timer_object,nanoseconds);
}

static inline uint32_t TimerMillisecondsToTicks(
	struct TimerObject *timer_object, 
	uint32_t milliseconds)
{
	return LldTimerMillisecondsToTicks(timer_object,milliseconds);
}

static inline uint32_t TimerSecondsToTicks(
	struct TimerObject *timer_object, 
	uint32_t seconds)
{
	return LldTimerSecondsToTicks(timer_object,seconds);
}

static inline uint32_t TimerHzToTicks(
	struct TimerObject *timer_object, 
	uint32_t hz)
{
	return LldTimerHzToTicks(timer_object,hz);
}


#endif
