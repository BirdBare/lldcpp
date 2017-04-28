//
//
//
//
//



#ifndef STM32F767_PWR_H
#define STM32F767_PWR_H

#include "board.h"
#include "rcc_lld.h"

struct PwrObject
{
	struct RccObject rcc;

	uint16_t unused2;

	volatile PWR_TypeDef * const pwr;
};

extern struct PwrObject
	PWR_OBJECT;









#endif
