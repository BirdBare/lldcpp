//
//
//
//
//



#include "dma_lld.h"


struct DmaObject
	DMA1_Stream0_OBJECT = {{0x30,21},0,0,DMA1_Stream0},
	DMA1_Stream1_OBJECT = {{0x30,21},0,6,DMA1_Stream1},
	DMA1_Stream2_OBJECT = {{0x30,21},0,16,DMA1_Stream2},
	DMA1_Stream3_OBJECT = {{0x30,21},0,22,DMA1_Stream3},
	DMA1_Stream4_OBJECT = {{0x30,21},4,0,DMA1_Stream4},
	DMA1_Stream5_OBJECT = {{0x30,21},4,6,DMA1_Stream5},
	DMA1_Stream6_OBJECT = {{0x30,21},4,16,DMA1_Stream6},
	DMA1_Stream7_OBJECT = {{0x30,21},4,22,DMA1_Stream7},

	DMA2_Stream0_OBJECT = {{0x30,22},0,0,DMA2_Stream0},
	DMA2_Stream1_OBJECT = {{0x30,22},0,6,DMA2_Stream1},
	DMA2_Stream2_OBJECT = {{0x30,22},0,16,DMA2_Stream2},
	DMA2_Stream3_OBJECT = {{0x30,22},0,22,DMA2_Stream3},
	DMA2_Stream4_OBJECT = {{0x30,22},4,0,DMA2_Stream4},
	DMA2_Stream5_OBJECT = {{0x30,22},4,6,DMA2_Stream5},
	DMA2_Stream6_OBJECT = {{0x30,22},4,16,DMA2_Stream6},
	DMA2_Stream7_OBJECT = {{0x30,22},4,22,DMA2_Stream7};




uint32_t DmaConfig(struct DmaObject *dma_stream_object, struct DmaConfig *dma_config)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_stream_object->dma_stream;
	//get dma_stream

	if((dma_stream->CR & DMA_SxCR_EN) == 1)
	{
		return;
		//if the stream is enabled then we cannot config
	}

	{
	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma

	uint32_t *flag_clear_register = 
		(uint32_t *)((uint32_t)(dma->LIFCR) + 
			dma_stream_object->flag_register_offset);
	//get dma flag clear register

	*flag_clear_register |= (uint32_t)0b111101 << dma_stream_object->flag_offset;
	//reset flags in the dma
	}
	//in bracket because variables were very temporary

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





