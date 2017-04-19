//
//
//
//
//



#include "ADC_lld.h"
#include "RCC_lld.h"

struct ADCxo
	ADC1o = {{0x44,8},0,ADC1},
	ADC2o = {{0x44,9},0,ADC2},
	ADC3o = {{0x44,10},0,ADC3};


int ADC_Config(struct ADCxo *ADCo, int CR1, int CR2, int HTR, int LTR,
	int SampHigh, int SampLow)
{
	volatile ADC_TypeDef * const ADCx = ADCo->ADCx;

	if((ADCx->CR2 & ADC_CR2_ADON) == 0)
	{
		ADCx->CR1 = CR1;
		ADCx->HTR = HTR;
		ADCx->LTR = LTR;
		ADCx->CR2 = CR2;
		ADCx->SMPR1 = SampHigh;
		ADCx->SMPR2 = SampLow;

		return 0;
	}
	return ADC_CONFIG_ENABLED;
}



int ADC_ConfigRegular(struct ADCxo *ADCo, int SeqHigh, int SeqMid, 
	int SeqLow)
{
	volatile ADC_TypeDef * const ADCx = ADCo->ADCx;

	if((ADCx->CR2 & ADC_CR2_ADON) == 0)
	{
		ADCx->SQR1 = SeqHigh;
		ADCx->SQR2 = SeqMid;
		ADCx->SQR3 = SeqLow;

		return 0;
	}
	return ADC_CONFIG_ENABLED;
}



int ADC_ConfigInjected(struct ADCxo *ADCo, int JSQR, int JOFR1, 
	int JOFR2, int JOFR3, int JOFR4)
{
	volatile ADC_TypeDef * const ADCx = ADCo->ADCx;

	if((ADCx->CR2 & ADC_CR2_ADON) == 0)
	{
		ADCx->CR2 = JSQR;
		ADCx->JOFR1 = JOFR1;
		ADCx->JOFR2 = JOFR2;
		ADCx->JOFR3 = JOFR3;
		ADCx->JOFR4 = JOFR4;

		return 0;
	}
	return ADC_CONFIG_ENABLED;
}




int ADC_ResetConfig(struct ADCxo *ADCo)
{
	volatile ADC_TypeDef * const ADCx = ADCo->ADCx;

	if((ADCx->CR2 & ADC_CR2_ADON) == 0)
	{
		RCC_Reset((struct RCCxo *)ADCo);

		return 0;
	}
	return ADC_CONFIG_ENABLED;
}

void ADC_Enable(struct ADCxo *ADCo)
{
	ADCo->ADCx->CR2 |= ADC_CR2_ADON;
}

void ADC_Disable(struct ADCxo *ADCo)
{	
	ADCo->ADCx->CR2 &= ~ADC_CR2_ADON;
}

int ADC_StartRegular(struct ADCxo *ADCo)
{
	volatile ADC_TypeDef * const ADCx = ADCo->ADCx;

	if((ADCx->CR2 & ADC_CR2_ADON) != 0)
	{
		ADCx->CR2 |= ADC_CR2_SWSTART;
		
		return 0;
	}
	return ADC_START_ADON;
}

int ADC_StartInjected(struct ADCxo *ADCo)
{
	volatile ADC_TypeDef * const ADCx = ADCo->ADCx;

	if((ADCx->CR2 & ADC_CR2_ADON) != 0)
	{
		ADCx->CR2 |= ADC_CR2_JSWSTART;
		
		return 0;
	}
	return ADC_START_ADON;
}


int ADC_Get16_Regular(struct ADCxo *ADCo)
{
	uint32_t ret;
	ASM(" ldrh %0, [%1, #0x4c]" : "=r" (ret) : "r" (ADCo->ADCx));
	return ret;
}


void ADC_Get16_Injected(struct ADCxo *ADCo, int DataArray[4])
{
	volatile ADC_TypeDef * const ADCx = ADCo->ADCx;

	DataArray[0] = ADCx->JDR1;
	DataArray[1] = ADCx->JDR2;
	DataArray[2] = ADCx->JDR3;
	DataArray[3] = ADCx->JDR4;
}





