//
//
//
//
//



#include "nvic_lld.h"

uint32_t NvicSetInterruptPriority(IRQn_Type IRQn, int PRIO)
{
	NVIC->IP[(uint32_t)IRQn] = (uint8_t)((PRIO << (8 -__NVIC_PRIO_BITS)) & 0xFF);

	return 0;
}

uint32_t NvicSetSystemInterruptPriority(IRQn_Type IRQn, int PRIO)
{
	SCB->SHP[((uint32_t)IRQn & 0xF) - 4] = 
		(uint8_t)((PRIO << (8 -__NVIC_PRIO_BITS)) & 0xFF);

	return 0;
}

uint32_t NvicGetInterruptPriority(IRQn_Type IRQn)
{
	return NVIC->IP[(uint32_t)IRQn] >> (8 -__NVIC_PRIO_BITS);
}

uint32_t NvicGetSystemInterruptPriority(IRQn_Type IRQn)
{
	return SCB->SHP[((uint32_t)IRQn & 0xF) - 4] >> (8 -__NVIC_PRIO_BITS);
}

void NvicEnableInterrupt(IRQn_Type IRQn)
{
	NVIC->ISER[(uint32_t)IRQn >> 5] = 1 << ((uint32_t)IRQn & 0x1F);
}

void NvicDisableInterrupt(IRQn_Type IRQn)
{
	NVIC->ICER[(uint32_t)IRQn >> 5] = 1 << ((uint32_t)IRQn & 0x1F);
}







