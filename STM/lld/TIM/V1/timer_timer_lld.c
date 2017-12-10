//
//
//
//
//



#include "timer_lld.h"

uint32_t LldTimerConfigTimerMilliseconds(struct TimerObject *timer_object,
	struct TimerConfig *timer_config)
{
	timer_object->timer_config = timer_config;
	//set timer_config

	volatile TIM_TypeDef * const timer = timer_object->timer;
	//get timer

	//Calculate timer counter registers
	uint64_t ticks, counts;
	uint32_t peripheral_speed = ClockGetPeripheralSpeed(&timer_object->rcc);
	//get initial speed

	if(ClockGetPeripheralPrescaler(&timer_object->rcc) != 0)
	{
		peripheral_speed <<= 1;
	}
	//special case where timer clock can be 2x bus speed if bus speed is scaled

	counts = ticks = (uint64_t)peripheral_speed * timer_config->milliseconds / 1000;
	//get total num ticks at first for both

	while(counts > ((1 << timer_object->max_counts) - 1))
	{
		counts >>= 1;
	}
	//get within ARR reg limits. hardware dependent ARR size

	timer->ARR = counts; 
	//set counter max
	
	uint32_t prescaler= (ticks / counts) - 1;
	//calculate other counter register
	//minus 1 because the register adds 1

	if(prescaler > 65535)
	{
		return 1;
	}
	//if numbers are out of limits then return error

	timer->PSC = prescaler;
	//set prescaler

	timer_config->milliseconds = (counts * prescaler * 10000 / peripheral_speed +
		5) / 10;
	//calculate actual milliseconds user will be getting
	//multiply by 10000, add 5, then divide by 10 is equal to rounding up by five
	//gives more accurate description of set time

	timer->DIER = 0;
	//disable interrupts temporarily because we are only updating registers
	timer->EGR |= TIM_EGR_UG;
	//upate prescaler and counter registers

	//FINISHED

	return 0;
}


uint32_t LldTimerStartTimerPolled(struct TimerObject *timer_object)
{
	volatile TIM_TypeDef * const timer = timer_object->timer;
	//get timer

	struct TimerConfig *timer_config = timer_object->timer_config;

	timer->SR = 0;
	//reset status register

	timer->DIER = (timer_config->callback != 0) ? TIM_DIER_UIE : 0 | 
		timer_config->dier;
	//enable update event interrupt if a callback is set. otherwise we just time

	timer->CR2 = timer_config->cr2;
	//set cr2 options

	timer->CR1 = timer_config->cr1 | TIM_CR1_CEN | TIM_CR1_OPM;
	//enable counter and one pulse mode and set timer options

		while((timer_object->timer->SR & TIM_SR_UIF) == 0)
		{
			asm("");
		};
		//poll timer flag to wait
		
		return 0;
}

uint32_t LldTimerStartTimerInterrupt(struct TimerObject *timer_object)
{

	struct TimerConfig *timer_config = timer_object->timer_config;

	if(timer_config->callback == 0)
	{
		return 1;
	}
	//check if callback is set. it has to be for interrupt timer mode

	volatile TIM_TypeDef * const timer = timer_object->timer;
	//get timer

	timer->SR = 0;
	//reset status register

	timer->DIER = TIM_DIER_UIE | timer_config->dier;
	//enable update event interrupt if a callback is set. otherwise we just time

	timer->CR2 = timer_config->cr2;
	//set cr2 options

	timer->CR1 = timer_config->cr1 | TIM_CR1_CEN;
	//enable counter and one pulse mode and set timer options

	return 0;
}


uint32_t LldTimerSetTimerMilliseconds(struct TimerObject *timer_object,
	struct TimerConfig *timer_config)
{
	timer_object->timer_config = timer_config;
	//set timer_config

	volatile TIM_TypeDef * const timer = timer_object->timer;
	//get timer

	//Calculate timer counter registers
	uint64_t ticks, counts;
	uint32_t peripheral_speed = ClockGetPeripheralSpeed(&timer_object->rcc);
	//get initial speed

	if(ClockGetPeripheralPrescaler(&timer_object->rcc) != 0)
	{
		peripheral_speed <<= 1;
	}
	//special case where timer clock can be 2x bus speed if bus speed is scaled

	counts = ticks = (uint64_t)peripheral_speed * timer_config->milliseconds / 1000;
	//get total num ticks at first for both

	while(counts > ((1 << timer_object->max_counts) - 1))
	{
		counts >>= 1;
	}
	//get within ARR reg limits. hardware dependent ARR size

	timer->ARR = counts; 
	//set counter max
	
	uint32_t prescaler= (ticks / counts) - 1;
	//calculate other counter register
	//minus 1 because the register adds 1

	if(prescaler > 65535)
	{
		return 1;
	}
	//if numbers are out of limits then return error

	timer->PSC = prescaler;
	//set prescaler

	timer_config->milliseconds = (counts * prescaler * 10000 / peripheral_speed +
		5) / 10;
	//calculate actual milliseconds user will be getting
	//multiply by 10000, add 5, then divide by 10 is equal to rounding up by five
	//gives more accurate description of set time

	
	timer->DIER = 0;
	//disable interrupts temporarily because we are only updating registers
	timer->EGR |= TIM_EGR_UG;
	//upate prescaler and counter registers

	timer->SR = 0;
	//reset status register

	timer->DIER = (timer_config->callback != 0) ? TIM_DIER_UIE : 0 | 
		timer_config->dier;
	//enable update event interrupt if a callback is set. otherwise we just time

	timer->CR2 = timer_config->cr2;
	//set cr2 options

	timer->CR1 = timer_config->cr1 | TIM_CR1_CEN;
	//enable counter and one pulse mode and set timer options

	//FINISHED

	return 0;
}

