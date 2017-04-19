//
//
//
//
//



#ifndef STM32F767_CRC_H
#define STM32F767_CRC_H

#include "board.h"
#include "BARE_DEFINES.h"
#include "RCC_lld.h"

struct CRCxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile CRC_TypeDef * const CRCx;
};

extern struct CRCxo
	CRCo;




int CRC_Config(struct CRCxo *CRCo, int CR, int INIT);

void CRC_ConfigPolynomial(struct CRCxo *CRCo, int POL);

int CRC_ResetConfig(struct CRCxo *CRCo);

ALWAYS_INLINE uint32_t CRC_Get32(struct CRCxo *CRCo)
{
	uint32_t ret;
	ASM(" ldr %0, [%1, #0x0]" : "=r" (ret) : "r" (CRCo->CRCx));
	return ret;
}

ALWAYS_INLINE void CRC_Put32(struct CRCxo *CRCo, uint32_t Data)
{
	ASM(" str %1, [%0, #0x0]" :: "r" (CRCo->CRCx), "r" (Data));
}






#endif
