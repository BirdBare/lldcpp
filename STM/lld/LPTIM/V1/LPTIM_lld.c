//
//
//
//
//



#include "LPTIM_lld.h"
#include "RCC_lld.h"

struct LPTIMxo
	LPTIM1o = {{0x40,9},0,LPTIM1};






int LPTIM_Config(struct LPTIMxo * LPTIMo, int IER, int CFGR)
{
	volatile LPTIM_TypeDef * const LPTIMx = LPTIMo->LPTIMx;

	if((LPTIMx->CR & LPTIM_CR_ENABLE) == 0)
	{
		LPTIMx->IER = IER;
		LPTIMx->CFGR = CFGR;

		return 0;
	}
	return LPTIM_CONFIG_ENABLED;
}

int LPTIM_ResetConfig(struct LPTIMxo * LPTIMo)
{
	if((LPTIMo->LPTIMx->CR & LPTIM_CR_ENABLE) == 0)
	{
		RCC_Reset((struct RCCxo *)LPTIMo);

		return 0;
	}
	return LPTIM_CONFIG_ENABLED;	
}

void LPTIM_Enable(struct LPTIMxo * LPTIMo)
{
	LPTIMo->LPTIMx->CR |= LPTIM_CR_ENABLE;
}


void LPTIM_Disable(struct LPTIMxo * LPTIMo)
{
	LPTIMo->LPTIMx->CR = 0; //set ENABLE to zero by setting whole reg to zero
}

void LPTIM_StartSingle(struct LPTIMxo * LPTIMo)
{
	LPTIMo->LPTIMx->CR = LPTIM_CR_ENABLE | LPTIM_CR_SNGSTRT;
}

void LPTIM_StartContinuous(struct LPTIMxo * LPTIMo)
{
	LPTIMo->LPTIMx->CR = LPTIM_CR_ENABLE | LPTIM_CR_CNTSTRT;
}






