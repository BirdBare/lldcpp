//
//
//
//
//



#ifndef STM32F767_RNG_H
#define STM32F767_RNG_H

#include "board.h"
#include "BARE_DEFINES.h"
#include "RCC_lld.h"

struct RNGxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile RNG_TypeDef * const RNGx;
};

extern struct RNGxo
	RNGo;

#define RNG_CONFIG_ENABLED 1
int RNG_Config(struct RNGxo *RNGo, int CR);
int RNG_ResetConfig(struct RNGxo *RNGo);
void RNG_Enable(struct RNGxo *RNGo);
void RNG_Disable(struct RNGxo *RNGo);

ALWAYS_INLINE int RNG_Get32(struct RNGxo *RNGo)
{
	uint32_t ret;
	ASM(" ldr %0, [%1, #0x8]" : "=r" (ret) : "r" (RNGo->RNGx));
	return ret;
}






#endif
