//
//
//
//
//



#ifndef DMA_HAL_H
#define DMA_HAL_H

#include "dma_lld.h"
#include "bareos.h"
#include "mutex.h"


//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t DmaInit(struct DmaObject * const dma_object)
{
	BareOSDisableInterrupts();

	if(dma_object->initialized != 0)
	{	
		BareOSEnableInterrupts();
		return 1;
	}

	if(LldDmaInit(dma_object) != 0)
	{
		BareOSEnableInterrupts();
		return 1;
	}

	LldDmaResetConfig(dma_object);

	MutexInit(&dma_object->mutex);
	
	dma_object->initialized = 1;
	
	BareOSEnableInterrupts();

	return 0;
}

static inline uint32_t DmaDeinit(struct DmaObject * const dma_object)
{

	BareOSDisableInterrupts();

	if(LldDmaDeinit(dma_object) != 0)
	{
		BareOSEnableInterrupts();
		return 1;
	}

	MutexDeinit(&dma_object->mutex);

	dma_object->initialized = 0;
	
	BareOSEnableInterrupts();

	return 0;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
static inline uint32_t DmaResetConfig(
	struct DmaObject * const dma_object)
{
	MutexLock(&dma_object->mutex);

	uint32_t ret = LldDmaResetConfig(dma_object);

	MutexUnlock(&dma_object->mutex);

	return ret;
}


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaTransfer(struct DmaObject *dma_object, 
	struct DmaConfig *dma_config);


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaSetMemory(struct DmaObject *dma_object, 
	struct DmaConfig* dma_config);




#endif
