//
//
//
//
//



#include "timer_lld.h"

uint32_t LldTimerConfigTimer(struct TimerObject *timer_object,
	struct TimerConfig *timer_config)
{
	timer_object->timer_config = timer_config;
	//set timer_config

	volatile TIM_TypeDef * const timer = timer_object->timer;
	//get timer

	//Calculate timer counter registers
	uint32_t ticks;
	uint32_t peripheral_speed = ClockGetPeripheralSpeed(&timer_object->rcc);
	//get initial speed

	if(ClockGetPeripheralPrescaler(&timer_object->rcc) != 0)
	{
		peripheral_speed <<= 1;
	}
	//special case where timer clock can be 2x bus speed if bus speed is scaled

	ticks = peripheral_speed / timer_config->clock_speed;
	//get total num ticks at first 

	if(ticks > ((1 << timer_object->max_counts) - 1))
	{
		return 1;;
	}
	//for timer. clock_speed must fit inside ARR register	

	timer->ARR = ticks; 
	//set in arr register because arr can be up to 32 bits long.
	
	timer_config->clock_speed = peripheral_speed / ticks;
	//calculate actual clock speed. can be off due to integer rounding

		//FINISHED

	return 0;
}

uint32_t LldTimerStartTimerPolled(struct TimerObject *timer_object, 
	uint32_t counts)
{

	if(counts > 65536)
	{
	 return 1;
	}
	//return if too large

	volatile TIM_TypeDef * const timer = timer_object->timer;
	//get timer

	struct TimerConfig *timer_config = timer_object->timer_config;
	//get timer config

	timer->PSC = counts - 1;
	//set prescaler

	timer->DIER = 0;
	//disable interrupts temporarily because we are only updating registers
	timer->EGR |= TIM_EGR_UG;
	//upate prescaler and counter registers

	timer->SR &= 0;
	//reset status register in a long way incase cpu is too fast

	timer->CR2 = timer_config->cr2;
	//set cr2 options

	timer->DIER = (timer_config->callback != 0) ? TIM_DIER_UIE : 0 | 
		timer_config->dier;
	//enable update event interrupt if a callback is set. otherwise we just time

	timer->CR1 = timer_config->cr1 | TIM_CR1_CEN | TIM_CR1_OPM;
	//enable counter and one pulse mode and set timer options

	while((timer_object->timer->SR & TIM_SR_UIF) == 0)
	{
		asm("");
	};
	//poll timer flag to wait
		
	return 0;
}

uint32_t LldTimerStartTimerInterrupt(struct TimerObject *timer_object,
	uint32_t counts)
{

	struct TimerConfig *timer_config = timer_object->timer_config;

	if(timer_config->callback == 0 || counts > 65536)
	{
		return 1;
	}
	//check if callback is set. it has to be for interrupt timer mode
	//return if too large

	volatile TIM_TypeDef * const timer = timer_object->timer;
	//get timer

	timer->PSC = counts - 1;
	//set prescaler

	timer->DIER = 0;
	//disable interrupts temporarily because we are only updating registers
	timer->EGR |= TIM_EGR_UG;
	//upate prescaler and counter registers

	timer->SR &= 0;
	//reset status register in a long way incase cpu is too fast

	timer->DIER = TIM_DIER_UIE | timer_config->dier;
	//enable update event interrupt if a callback is set. otherwise we just time

	timer->CR2 = timer_config->cr2;
	//set cr2 options

	timer->CR1 = timer_config->cr1 | TIM_CR1_CEN;
	//enable counter and one pulse mode and set timer options

	return 0;
}





