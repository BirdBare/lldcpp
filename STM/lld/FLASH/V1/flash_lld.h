//
//
//
//
//



#ifndef FLASH_H
#define FLASH_H

#include "board.h"

struct FlashObject
{
	volatile FLASH_TypeDef * const flash;
};

extern struct FlashObject FLASH_OBJECT;


ALWAYS_INLINE void LldFlashInit(const struct FlashObject * const flash_object)
{
	flash_object->flash->ACR = 
		FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;
}

struct FlashConfig
{
	uint32_t cpu_speed;
};



ALWAYS_INLINE void LldFlashConfig(
const struct FlashObject * const flash_object,
const struct FlashConfig * const flash_config)
{
	flash_object->flash->ACR |= flash_config->cpu_speed / 30000000;
	//cpu speed is in hz so divide by 1000000. then if we divide by 30 it gives
	//wait states needed
}




#endif
