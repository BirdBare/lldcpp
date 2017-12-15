//
//
//
//
//



#ifndef TIMER_HAL_H
#define TIMER_HAL_H

#include "timer_lld.h"

// TimerSetTimerPolled
// TimerSetTimerInterrupt

// TimerEnablePwm
// TimerSetDutyCycle

// Timer


static inline void LldTimerInit(struct TimerObject *timer_object)
{
	RccEnableClock(&timer_object->rcc);
}

//Timer

uint32_t TimerConfigTimerMilliseconds(
	struct TimerObject *timer_object,
	struct TimerConfig *timer_config);

uint32_t TimerStartTimerPolled(struct TimerObject *timer_object);
uint32_t TimerStartTimerInterrupt(struct TimerObject *timer_object);
//End

void TimerStop(struct TimerObject *timer_object);

	


#endif
