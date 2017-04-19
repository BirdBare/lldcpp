//
//
//
//
//



#include "FLASH_lld.h"


void FLASH_UnlockControl(void)
{
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xcdef89ab;
}

int FLASH_ConfigControl(int CR)
{
	if((FLASH->CR & FLASH_CR_LOCK) == 0)
	{
		FLASH->CR = CR;
		
		return 0;
	}
	return FLASH_CONFIG_LOCK;
}



void FLASH_UnlockOption(void)
{
	FLASH->OPTKEYR = 0x08192a3b;
	FLASH->OPTKEYR = 0x4c5d6e7f;
}

int FLASH_ConfigOption(int OPTCR, int OPTCR1)
{
	if((FLASH->OPTCR & FLASH_OPTCR_OPTLOCK) == 0)
	{
		FLASH->OPTCR = OPTCR;
		FLASH->OPTCR1 = OPTCR1;
		
		return 0;
	}
	return FLASH_CONFIG_LOCK;
}







