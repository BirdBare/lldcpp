//
//
//
//
//



#include "dma_hal.h"


uint32_t DmaTransfer(struct DmaObject *dma_object, 
	struct DmaConfig	*dma_config)
{
	MutexLock(&dma_object->mutex);

	dma_config->callback = (void *)&BareOSSchedulerAddThread;
	dma_config->args = BareOSSchedulerGetCurrentThread();

	uint32_t ret  = LldDmaTransfer(dma_object, dma_config);

	if(ret == 0)
	{
		BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
		BareOSCallSwitch();
	}

	MutexUnlock(&dma_object->mutex);
	
	return ret;
}


uint32_t DmaSetMemory(struct DmaObject *dma_object, 
	struct DmaConfig	*dma_config)
{
	MutexLock(&dma_object->mutex);

	dma_config->callback = (void *)&BareOSSchedulerAddThread;
	dma_config->args = BareOSSchedulerGetCurrentThread();

	uint32_t ret = LldDmaSetMemory(dma_object, dma_config);

	if(ret == 0)
	{
		BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
		BareOSCallSwitch();
	}

	MutexUnlock(&dma_object->mutex);
	
	return ret;
}


