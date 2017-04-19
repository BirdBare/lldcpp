//
//
//
//
//



#ifndef STM32F767_SYSCFG_H
#define STM32F767_SYSCFG_H

#include "board.h"
#include "BARE_DEFINES.h"
#include "RCC_lld.h"

struct SYSCFGxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile SYSCFG_TypeDef * const SYSCFGx;
};

extern struct SYSCFGxo
	SYSCFGo;

void SYSCFG_Config(struct SYSCFGxo *SYSCFGo, int MEMRMP, int CBR, int CMPCR);

ALWAYS_INLINE void SYSCFG_ConfigMode(struct SYSCFGxo *SYSCFGo, int PMC)
{
	ASM(" str %0, [%1, #0x4]" :: "r" (PMC), "r" (SYSCFGo->SYSCFGx));
}

void SYSCFG_ConfigEXTI(struct SYSCFGxo *SYSCFGo, int EXTIHigh, int EXTIMidHigh, 
	int EXTIMidLow, int EXTILow);





#endif
