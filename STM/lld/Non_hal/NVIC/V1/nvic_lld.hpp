//
//
//
//
//



#ifndef NVIC_H
#define NVIC_H

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif


struct NvicHal
{
	const uint32_t num_irq; //number of interrupts to enable in for the peripheral

	const IRQn_Type *irq_number; //IRQn_Type from stm32 driver definitions header
};

uint32_t NvicSetInterruptPriority(IRQn_Type IRQn, int PRIO);
uint32_t NvicSetSystemInterruptPriority(IRQn_Type IRQn, int PRIO);
uint32_t NvicGetInterruptPriority(IRQn_Type IRQn);
uint32_t NvicGetSystemInterruptPriority(IRQn_Type IRQn);

void NvicEnableInterrupt(IRQn_Type IRQn);

void NvicDisableInterrupt(IRQn_Type IRQn);

static inline uint32_t NvicEnableHalInterrupt(
	const struct NvicHal *nvic_object)
{
	for(uint32_t i = 0; i < nvic_object->num_irq; i++)
	{
		NvicEnableInterrupt(nvic_object->irq_number[i]);
	}
		
	return 0;
}

static inline uint32_t NvicDisableHalInterrupt(
	const struct NvicHal *nvic_object)
{
	for(uint32_t i = 0; i < nvic_object->num_irq; i++)
	{
		NvicDisableInterrupt(nvic_object->irq_number[i]);
	}
		
	return 0;
}



static inline uint32_t NvicSetHalInterruptPriority(
	const struct NvicHal *nvic_object,
	uint8_t PRIO[])
{
	for(uint32_t i = 0; i < nvic_object->num_irq; i++)
	{
		NvicSetInterruptPriority(nvic_object->irq_number[i], PRIO[i]);
	}
		
	return 0;
}

static inline uint32_t NvicGetHalInterruptPriority(
	const struct NvicHal *nvic_object,
	uint8_t PRIO[])
{
	for(uint32_t i = 0; i < nvic_object->num_irq; i++)
	{
		PRIO[i] = NvicGetInterruptPriority(nvic_object->irq_number[i]);
	}
		
	return 0;
}

#ifdef __cplusplus
}
#endif



#endif
