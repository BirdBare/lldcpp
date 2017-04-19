//
//
//
//
//



#ifndef STM32F767_FLASH_H
#define STM32F767_FLASH_H

#include "board.h"
#include "BARE_DEFINES.h"
//#include "STM32F767_.h"

ALWAYS_INLINE void FLASH_ConfigAccess(int ACR)
{
	ASM(" str %0, [%1, #0x0]" :: "r" (ACR), "r" (FLASH));
}

ALWAYS_INLINE void FLASH_LockControl(void)
{
	uint32_t lock = 0b1 << 31;
	ASM(" str %0, [%1, #0x10]" :: "r" (lock), "r" (FLASH));	
}
#define FLASH_CONFIG_LOCK 1

void FLASH_UnlockControl(void);
int FLASH_ConfigControl(int CR);


ALWAYS_INLINE void FLASH_LockOption(void)
{
	uint32_t lock = 0b1;
	ASM(" str %0, [%1, #0x14]" :: "r" (lock), "r" (FLASH));	
}

void FLASH_UnlockOption(void);
int FLASH_ConfigOption(int OPTCR, int OPTCR1);








#endif
