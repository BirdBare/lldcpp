//
//
//
//
//



#include "RNG_lld.h"
#include "RCC_lld.h"

struct RNGxo
	RNGo = {{0x34,6},0,RNG};

int RNG_Config(struct RNGxo *RNGo, int CR)
{
	volatile RNG_TypeDef * const RNGx = RNGo->RNGx;

	if((RNGx->CR & RNG_CR_RNGEN) == 0)
	{
		RNGx->CR = CR;

		return 0;
	}
	return RNG_CONFIG_ENABLED;
}

int RNG_ResetConfig(struct RNGxo *RNGo)
{
	
	if((RNGo->RNGx->CR & RNG_CR_RNGEN) == 0)
	{
		RCC_Reset((struct RCCxo *)RNGo);

		return 0;
	}
	return RNG_CONFIG_ENABLED;
}


void RNG_Enable(struct RNGxo *RNGo)
{
	RNGo->RNGx->CR |= RNG_CR_RNGEN;
}

void RNG_Disable(struct RNGxo *RNGo)
{
	RNGo->RNGx->CR &= ~RNG_CR_RNGEN;
}





