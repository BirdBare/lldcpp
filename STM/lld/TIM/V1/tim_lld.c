//
//
//
//
//



#include "tim_lld.h"

struct TimerObject
	TIM1_OBJECT = {{0x44,0},0,TIM1},
	TIM2_OBJECT = {{0x40,0},1,TIM2},
	TIM3_OBJECT = {{0x40,1},0,TIM3},
	TIM4_OBJECT = {{0x40,2},0,TIM4},
	TIM5_OBJECT = {{0x40,3},1,TIM5},
	TIM6_OBJECT = {{0x40,4},0,TIM6},
	TIM7_OBJECT = {{0x40,5},0,TIM7},
	TIM8_OBJECT = {{0x44,1},0,TIM8},
	TIM9_OBJECT = {{0x44,16},0,TIM9},
	TIM10_OBJECT = {{0x44,17},0,TIM10},
	TIM11_OBJECT = {{0x44,18},0,TIM11},
	TIM12_OBJECT = {{0x40,6},0,TIM12},
	TIM13_OBJECT = {{0x40,7},0,TIM13},
	TIM14_OBJECT = {{0x40,8},0,TIM14};


uint32_t LldTimerConfigTick(
	struct TimerObject *timer_object, 
	struct TimerConfig *timer_config) 
{
	volatile TIM_TypeDef * const timer = timer_object->timer;
	//get timer

//Calculate timer counter registers
	uint64_t counts, ticks;

	counts = ticks = timer_config->ticks;
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

	timer_config->ticks = counts * prescaler;
	//calculate actual ticks user will be getting
//Finished

	return 0;
}





/*
void TIM_ConfigTime(struct TimObject *TIMo, uint32_t PSC, uint32_t ARR)
{
	volatile TIM_TypeDef * const tim = TIMo->tim;

		tim->ARR = ARR;
		tim->PSC = PSC;
}






int TIM_ConfigChannel(struct TimObject *TIMo, int Channel, int DIER, int CCMR, 
	int CCER)
{
	volatile TIM_TypeDef * const tim = TIMo->tim;

	if((tim->CCER & TIM_CCER_CC1E) == 0)
	{
		tim->DIER = (tim->DIER & ~(0b0000001000000010 << Channel)) 
			| (DIER << Channel);

		Channel--;

		tim->CCER = ((tim->CCER & ~(0b0000000000001111 << (Channel << 2))) 
			| (CCER << (Channel << 2)));

		uint32_t temp = (Channel & 1) << 3;
		//get bits offset from channel

		CCMR <<= temp;
		//shift CCMR to correct position for channel

		temp = ~(0b10000000011111111 << temp);
		//create bit mask for CCMR reg

		__IO uint32_t *CCMRreg = &tim->CCMR1;
		//get CCMR1

		switch(Channel >> 1)
		{
			case 2:
				CCMRreg = (void *)((uint32_t)CCMRreg + 0x38);
				//get to CCMR3

			case 1:
				CCMRreg = (void *)((uint32_t)CCMRreg + 0x4);				
				//get to CCMR2
		}
		
		*CCMRreg |= (*CCMRreg & temp) | CCMR;

		return 0;
	}
	return TIM_CONFIG_ENABLED;
}

int TIM_ResetConfig(struct TimObject *TIMo)
{
	if((TIMo->tim->CR1 & TIM_CR1_CEN) == 0)
	{
		RccResetPeripheral(&TIMo->rcc);
		return 0;
	}
	return TIM_CONFIG_ENABLED;
}

void TIM_Enable(struct TimObject *TIMo)
{
	TIMo->tim->CR1 |= TIM_CR1_CEN;
}

void TIM_EnableChannel(struct TimObject *TIMo, int Channel)
{
	TIMo->tim->CCER |= TIM_CCER_CC1E << ((Channel - 1) << 2);
}

void TIM_Disable(struct TimObject *TIMo)
{
	TIMo->tim->CR1 &= ~TIM_CR1_CEN;
}

void TIM_DisableChannel(struct TimObject *TIMo, int Channel)
{
	TIMo->tim->CCER &= ~(TIM_CCER_CC1E << ((Channel - 1) << 2));
}
*/



