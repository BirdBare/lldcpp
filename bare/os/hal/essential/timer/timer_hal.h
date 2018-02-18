//
//
//
//
//



#ifndef TIMER_HAL_H
#define TIMER_HAL_H

#include "timer_lld.h"
#include "bareos.h"

// TimerSetTimerPolled
// TimerSetTimerInterrupt

// TimerEnablePwm
// TimerSetDutyCycle

// Timer


static inline void TimerInit(struct TimerObject *timer_object)
{
	BareOSDisableInterrupts();
	LldTimerInit(timer_object);
	BareOSEnableInterrupts();
}

//Timer

uint32_t TimerConfigTimer(
	struct TimerObject *timer_object,
	struct TimerConfig *timer_config);

uint32_t TimerStartTimerPolled(struct TimerObject *timer_object,
	uint32_t counts);

uint32_t TimerStartTimerInterrupt(struct TimerObject *timer_object,
	uint32_t counts);
//End

void TimerStop(struct TimerObject *timer_object);

	


#endif
