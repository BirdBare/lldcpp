//
//
//
//
//



#ifndef TIM_LLD_H
#define TIM_LLD_H

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
	struct RccObject rcc;

	uint8_t max_counts; //indicator if timer has 16 bit or 32 bit register size 
											//o is 16 bit and 1 is 32 bit

	uint8_t:8;

	volatile TIM_TypeDef * const timer;
};

extern struct TimerObject
	TIM1_OBJECT,
	TIM2_OBJECT,
	TIM3_OBJECT,
	TIM4_OBJECT,
	TIM5_OBJECT,
	TIM6_OBJECT,
	TIM7_OBJECT,
	TIM8_OBJECT,
	TIM9_OBJECT,
	TIM10_OBJECT,
	TIM11_OBJECT,
	TIM12_OBJECT,
	TIM13_OBJECT,
	TIM14_OBJECT;

struct TimerConfig
{
	//REQUIRED SETTINGS
	uint32_t ticks; 
													 

	//DRIVER SETTINGS
	union
	{
		uint16_t cr1;
	};
	union
	{
		uint16_t cr2;
	};
	union
	{
		uint16_t dier;
	};
	uint16_t:16;
	//END

};

static inline uint32_t LldTimerNanosecondsToTicks(
struct TimerObject *timer_object, 
	uint32_t nanoseconds)
{
	return (uint64_t)ClockGetPeripheralSpeed(&timer_object->rcc) * 
		nanoseconds / 1000000;
	//hz * seconds = ticks. nanoseconds to seconds is a factor of 1000000
}

static inline uint32_t LldTimerMillisecondsToTicks(
	struct TimerObject *timer_object, 
	uint32_t milliseconds)
{
	return (uint64_t)ClockGetPeripheralSpeed(&timer_object->rcc) * 
		milliseconds / 1000;
	//hz * seconds = ticks. milliseconds to seconds is a factor of 1000
}

static inline uint32_t LldTimerSecondsToTicks(
	struct TimerObject *timer_object, 
	uint32_t seconds)
{
	return (uint64_t)ClockGetPeripheralSpeed(&timer_object->rcc) * seconds;
	//hz * seconds = ticks. 
}

static inline uint32_t LldTimerHzToTicks(
	struct TimerObject *timer_object, 
	uint32_t hz)
{
	return ClockGetPeripheralSpeed(&timer_object->rcc) / hz;
}


#define TIM_CONFIG_ENABLED 1

uint32_t LldTimerConfigOverflow(
	struct TimerObject *timer_object, 
	struct TimerConfig *timer_config); 

	


/*
void TIM_ConfigTime(struct TimObject *TIMo, uint32_t PSC, uint32_t ARR);

int TIM_ConfigChannel(struct TimObject *TIMo, int Channel, int DIER, int CCMR, 
	int CCER);

int TIM_ResetConfig(struct TimObject *TIMo);

void TIM_Enable(struct TimObject *TIMo);

void TIM_EnableChannel(struct TimObject *TIMo, int Channel);

void TIM_Disable(struct TimObject *TIMo);

void TIM_DisableChannel(struct TimObject *TIMo, int Channel);

void TIM_Get32(struct TimObject *TIMo, int DataArray[6]);
*/

#endif
