//
//
//
//
//



#include "dma_lld.h"


struct DmaObject
	DMA1_STREAM0_OBJECT = {{0x30,21},0,0,DMA1_Stream0},
	DMA1_STREAM1_OBJECT = {{0x30,21},0,6,DMA1_Stream1},
	DMA1_STREAM2_OBJECT = {{0x30,21},0,16,DMA1_Stream2},
	DMA1_STREAM3_OBJECT = {{0x30,21},0,22,DMA1_Stream3},
	DMA1_STREAM4_OBJECT = {{0x30,21},4,0,DMA1_Stream4},
	DMA1_STREAM5_OBJECT = {{0x30,21},4,6,DMA1_Stream5},
	DMA1_STREAM6_OBJECT = {{0x30,21},4,16,DMA1_Stream6},
	DMA1_STREAM7_OBJECT = {{0x30,21},4,22,DMA1_Stream7},

	DMA2_STREAM0_OBJECT = {{0x30,22},0,0,DMA2_Stream0},
	DMA2_STREAM1_OBJECT = {{0x30,22},0,6,DMA2_Stream1},
	DMA2_STREAM2_OBJECT = {{0x30,22},0,16,DMA2_Stream2},
	DMA2_STREAM3_OBJECT = {{0x30,22},0,22,DMA2_Stream3},
	DMA2_STREAM4_OBJECT = {{0x30,22},4,0,DMA2_Stream4},
	DMA2_STREAM5_OBJECT = {{0x30,22},4,6,DMA2_Stream5},
	DMA2_STREAM6_OBJECT = {{0x30,22},4,16,DMA2_Stream6},
	DMA2_STREAM7_OBJECT = {{0x30,22},4,22,DMA2_Stream7};




uint32_t DmaConfig(struct DmaObject *dma_stream_object, struct DmaConfig *dma_config)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_stream_object->dma_stream;
	//get dma_stream

	if((dma_stream->CR & DMA_SxCR_EN) == 1)
	{
		return 1;
		//if the stream is enabled then we cannot config
	}

	DmaClearFlags(dma_stream_object,0b111101);
	//reset flags in the dma

	dma_stream->NDTR = dma_config->ndtr;
	dma_stream->PAR = dma_config->par;
	dma_stream->M0AR = dma_config->m0ar;
	dma_stream->M1AR = dma_config->m1ar;
	dma_stream->FCR = dma_config->fcr;
	dma_stream->CR = dma_config->cr | DMA_SxCR_EN;
	//configure and enable the dma stream

	return 0;
}

void DmaEnable(struct DmaObject *dma_stream_object)
{
	dma_stream_object->dma_stream->CR |= DMA_SxCR_EN;
}

void DmaDisable(struct DmaObject *dma_stream_object)
{
	dma_stream_object->dma_stream->CR &= ~DMA_SxCR_EN;
}


//******************************************************************************
//	
//									DMA IRQ HANDLERS	 
//	
//******************************************************************************

ALWAYS_INLINE void DMA_STREAM_HANDLER(struct DmaObject *dma_stream_object)
{
	EventSignalFlags(&dma_stream_object->event, DmaGetFlags(dma_stream_object));	
}

#ifdef DMA1
void DMA1_Stream0_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM0_OBJECT);
}
void DMA1_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM1_OBJECT);
}

void DMA1_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM2_OBJECT);
}

void DMA1_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM3_OBJECT);
}

void DMA1_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM4_OBJECT);
}

void DMA1_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM5_OBJECT);
}

void DMA1_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM6_OBJECT);
}

void DMA1_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM7_OBJECT);
}

#endif

#ifdef DMA2
void DMA2_Stream0_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2_STREAM1_OBJECT);
}

void DMA2_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2_STREAM2_OBJECT);
}

void DMA2_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2_STREAM3_OBJECT);
}

void DMA2_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2_STREAM4_OBJECT);
}

void DMA2_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2_STREAM5_OBJECT);
}

void DMA2_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2_STREAM6_OBJECT);
}

void DMA2_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2_STREAM7_OBJECT);
}

void DMA2_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1_STREAM0_OBJECT);
}
//##############################################################################

#endif

