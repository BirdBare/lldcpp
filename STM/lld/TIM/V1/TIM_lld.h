//
//
//
//
//



#ifndef STM32F767_TIM_H
#define STM32F767_TIM_H

#include "board.h"
#include "RCC_lld.h"

struct TIMxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile TIM_TypeDef * const TIMx;
};

extern struct TIMxo
	TIM1o,
	TIM2o,
	TIM3o,
	TIM4o,
	TIM5o,
	TIM6o,
	TIM7o,
	TIM8o,
	TIM9o,
	TIM10o,
	TIM11o,
	TIM12o,
	TIM13o,
	TIM14o;

#define TIM_CONFIG_ENABLED 1

int TIM_Config(struct TIMxo *TIMo, int CR1, int CR2, int SMCR, int DIER); 

void TIM_ConfigTime(struct TIMxo *TIMo, uint32_t PSC, uint32_t ARR);

int TIM_ConfigChannel(struct TIMxo *TIMo, int Channel, int DIER, int CCMR, 
	int CCER);

int TIM_ResetConfig(struct TIMxo *TIMo);

void TIM_Enable(struct TIMxo *TIMo);

void TIM_EnableChannel(struct TIMxo *TIMo, int Channel);

void TIM_Disable(struct TIMxo *TIMo);

void TIM_DisableChannel(struct TIMxo *TIMo, int Channel);

void TIM_Get32(struct TIMxo *TIMo, int DataArray[6]);


#endif
