//
//
//
//
//



#ifndef TIMER_LLD_H
#define TIMER_LLD_H

#include "board.h"
#include "rcc_lld.h"
#include "clock_lld.h"


// TimerSetTimerPolled
// TimerSetTimerInterrupt

// TimerEnablePwm
// TimerSetDutyCycle

// Timer


struct TimerObject
{
#ifdef USE_BAREOS

#endif

	struct RccObject rcc;

	uint8_t max_counts; //indicator if timer has 16 bit or 32 bit register size 

	uint8_t:8;

	volatile TIM_TypeDef * const timer;

	struct TimerConfig *timer_config;
};

extern struct TimerObject
	TIMER1_OBJECT,
	TIMER2_OBJECT,
	TIMER3_OBJECT,
	TIMER4_OBJECT,
	TIMER5_OBJECT,
	TIMER6_OBJECT,
	TIMER7_OBJECT,
	TIMER8_OBJECT,
	TIMER9_OBJECT,
	TIMER10_OBJECT,
	TIMER11_OBJECT,
	TIMER12_OBJECT,
	TIMER13_OBJECT,
	TIMER14_OBJECT;

struct TimerConfig
{
	uint32_t tick_frequency; //preferred speed of the timer in hz

	void (*callback)(void *args); //optional function called at end of interrupt 
	void *args; //args for callback
						
	uint8_t duty_cycle; //duty cycle in percent for pwm mode


	uint8_t:8;
	union
	{
		uint16_t cr1;
		
		struct
		{
			//LSB
			uint16_t:3;
			uint16_t one_pulse:1; //single count mode enable. counts only once.
			uint16_t:12;
			//MSB
		};

	};
	union
	{
		uint16_t cr2;
	};
	union
	{
		uint16_t dier;
	};

};




static inline uint32_t LldTimerInit(struct TimerObject *timer_object)
{
	RccEnableClock(&timer_object->rcc);

	return 0;
}

static inline uint32_t LldTimerDeinit(struct TimerObject *timer_object)
{
	RccDisableClock(&timer_object->rcc);

	return 0;
}

static inline uint32_t LldTimerResetConfig(struct TimerObject *timer_object)
{
	RccResetPeripheral(&timer_object->rcc);

	return 0;
}
//Timer

uint32_t LldTimerConfigTimer(
	struct TimerObject *timer_object,
	struct TimerConfig *timer_config);

uint32_t LldTimerStartTimerPolled(struct TimerObject *timer_object,
	uint32_t counts);
uint32_t LldTimerStartTimerInterrupt(struct TimerObject *timer_object,
	uint32_t counts);
//End

void TimerStop(struct TimerObject *timer_object);

	

#endif
