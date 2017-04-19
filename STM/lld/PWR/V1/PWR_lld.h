//
//
//
//
//



#ifndef STM32F767_PWR_H
#define STM32F767_PWR_H

#include "board.h"
#include "RCC_lld.h"

struct PWRxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile PWR_TypeDef * const PWRx;
};

extern struct PWRxo
	PWRo;

void PWR_Config(struct PWRxo *PWRo, int CR1, int CR2, int CSR2);

#define PWR_ENABLE_CLOCK 1
int PWR_EnableOverdrive(struct PWRxo *PWRo);


int PWR_DisableOverdrive(struct PWRxo *PWRo);








#endif
