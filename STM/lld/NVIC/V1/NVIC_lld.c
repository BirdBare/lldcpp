//
//
//
//
//



#include "NVIC_lld.h"

void NVIC_Config(IRQn_Type IRQn, int PRIO)
{
	NVIC->IP[(uint32_t)IRQn] = (uint8_t)((PRIO << (8 -__NVIC_PRIO_BITS)) & 0xFF);
}

void NVIC_ConfigSystem(IRQn_Type IRQn, int PRIO)
{
	SCB->SHPR[((uint32_t)IRQn & 0xF) - 4] = 
		(uint8_t)((PRIO << (8 -__NVIC_PRIO_BITS)) & 0xFF);
}

uint32_t NVIC_GetConfig(IRQn_Type IRQn)
{
	return NVIC->IP[(uint32_t)IRQn] >> (8 -__NVIC_PRIO_BITS);
}

uint32_t NVIC_GetConfigSystem(IRQn_Type IRQn)
{
	return SCB->SHPR[((uint32_t)IRQn & 0xF) - 4] >> (8 -__NVIC_PRIO_BITS);
}

void NVIC_Enable(IRQn_Type IRQn)
{
	NVIC->ISER[(uint32_t)IRQn >> 5] = 1 << ((uint32_t)IRQn & 0x1F);
}

void NVIC_Disable(IRQn_Type IRQn)
{
	NVIC->ICER[(uint32_t)IRQn >> 5] = 1 << ((uint32_t)IRQn & 0x1F);
}







