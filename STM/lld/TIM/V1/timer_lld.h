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
	uint32_t clock_speed; //preferred speed of the timer in hz

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




static inline void LldTimerInit(struct TimerObject *timer_object)
{
	RccEnableClock(&timer_object->rcc);
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
