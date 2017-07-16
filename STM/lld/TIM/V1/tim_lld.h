//
//
//
//
//



#ifndef TIM_H
#define TIM_H

#include "board.h"
#include "rcc_lld.h"

struct TimObject
{
	struct RccObject rcc;

	uint16_t unused2;

	volatile TIM_TypeDef * const tim;
};

extern struct TimObject
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

#define TIM_CONFIG_ENABLED 1

int TIM_Config(struct TimObject *TIMo, int CR1, int CR2, int SMCR, int DIER); 

void TIM_ConfigTime(struct TimObject *TIMo, uint32_t PSC, uint32_t ARR);

int TIM_ConfigChannel(struct TimObject *TIMo, int Channel, int DIER, int CCMR, 
	int CCER);

int TIM_ResetConfig(struct TimObject *TIMo);

void TIM_Enable(struct TimObject *TIMo);

void TIM_EnableChannel(struct TimObject *TIMo, int Channel);

void TIM_Disable(struct TimObject *TIMo);

void TIM_DisableChannel(struct TimObject *TIMo, int Channel);

void TIM_Get32(struct TimObject *TIMo, int DataArray[6]);


#endif
