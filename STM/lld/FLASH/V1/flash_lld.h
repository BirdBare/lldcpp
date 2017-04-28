//
//
//
//
//



#ifndef FLASH_H
#define FLASH_H

#include "board.h"
#include "bare_defines.h"
#include "flash_lld.h"

struct FlashObject
{

	volatile FLASH_TypeDef * const flash;

};

extern struct FlashObject FLASH_OBJECT;



ALWAYS_INLINE void FlashEnableArt(const struct FlashObject * const flash_object)
{
	flash_object->flash->ACR |= 
		FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;
}

struct FlashConfig
{
	uint32_t latency:4; //CPU read latency for flash
	//READ REFERENCE MANUAL TO FIND CORRECT VALUE

	uint32_t :28;
};



ALWAYS_INLINE void FlashConfig(
const struct FlashObject * const flash_object,
const struct FlashConfig * const flash_config)
{
	flash_object->flash->ACR |= flash_config->latency;
}




#endif
