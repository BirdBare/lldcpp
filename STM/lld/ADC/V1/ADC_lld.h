//
//
//
//
//



#ifndef STM32F767_ADC_H
#define STM32F767_ADC_H

#include "board.h"
#include "BARE_STM32F767.h"
#include "RCC_lld.h"



struct ADCxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile ADC_TypeDef * const ADCx;
};

extern struct ADCxo
	ADC1o,
	ADC2o,
	ADC3o;


#define ADC_CONFIG_ENABLED 1

int ADC_Config(struct ADCxo *ADCo, int CR1, int CR2, int HTR, int LTR,
	int SampHigh, int SampLow);

int ADC_ConfigRegular(struct ADCxo *ADCo, int SeqHigh, int SeqMid, 
	int SeqLow);

int ADC_ConfigInjected(struct ADCxo *ADCo, int JSQR, int JOFR1, 
	int JOFR2, int JOFR3, int JOFR4);

int ADC_ResetConfig(struct ADCxo *ADCo);

void ADC_Enable(struct ADCxo *ADCo);

void ADC_Disable(struct ADCxo *ADCo);

#define ADC_START_ADON 1
int ADC_StartRegular(struct ADCxo *ADCo);

int ADC_StartInjected(struct ADCxo *ADCo);

int ADC_Get16_Regular(struct ADCxo *ADCo);

void ADC_Get16_Injected(struct ADCxo *ADCo, int DataArray[4]);








#endif




