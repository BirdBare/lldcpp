//
//
//
//
//



#ifndef FLASH_HAL_H
#define FLASH_HAL_H

#include "flash_lld.h"
#include "bareos.h"

static inline void FlashInit(struct FlashObject *flash_object)
{
	LldFlashInit(flash_object);
}

static inline void FlashConfig(struct FlashObject *flash_object,
	struct FlashConfig *flash_config)
{
	LldFlashConfig(flash_object,flash_config);
}
	


#endif
