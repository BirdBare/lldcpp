//
//
//
//
//



#include "dma_lld.h"


struct DmaObject
	DMA1S0_OBJECT = {{0x30,21},0,0,DMA1_Stream0},
	DMA1S1_OBJECT = {{0x30,21},0,6,DMA1_Stream1},
	DMA1S2_OBJECT = {{0x30,21},0,16,DMA1_Stream2},
	DMA1S3_OBJECT = {{0x30,21},0,22,DMA1_Stream3},
	DMA1S4_OBJECT = {{0x30,21},4,0,DMA1_Stream4},
	DMA1S5_OBJECT = {{0x30,21},4,6,DMA1_Stream5},
	DMA1S6_OBJECT = {{0x30,21},4,16,DMA1_Stream6},
	DMA1S7_OBJECT = {{0x30,21},4,22,DMA1_Stream7},

	DMA2S0_OBJECT = {{0x30,22},0,0,DMA2_Stream0},
	DMA2S1_OBJECT = {{0x30,22},0,6,DMA2_Stream1},
	DMA2S2_OBJECT = {{0x30,22},0,16,DMA2_Stream2},
	DMA2S3_OBJECT = {{0x30,22},0,22,DMA2_Stream3},
	DMA2S4_OBJECT = {{0x30,22},4,0,DMA2_Stream4},
	DMA2S5_OBJECT = {{0x30,22},4,6,DMA2_Stream5},
	DMA2S6_OBJECT = {{0x30,22},4,16,DMA2_Stream6},
	DMA2S7_OBJECT = {{0x30,22},4,22,DMA2_Stream7};






//******************************************************************************
//	
//									DMA IRQ HANDLERS	 
//	
//******************************************************************************

ALWAYS_INLINE void DMA_STREAM_HANDLER(struct DmaObject *dma_stream_object)
{
return;
}

#ifdef DMA1
void DMA1_Stream0_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S0_OBJECT);
}
void DMA1_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S1_OBJECT);
}

void DMA1_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S2_OBJECT);
}

void DMA1_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S3_OBJECT);
}

void DMA1_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S4_OBJECT);
}

void DMA1_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S5_OBJECT);
}

void DMA1_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S6_OBJECT);
}

void DMA1_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S7_OBJECT);
}

#endif

#ifdef DMA2
void DMA2_Stream0_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S1_OBJECT);
}

void DMA2_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S2_OBJECT);
}

void DMA2_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S3_OBJECT);
}

void DMA2_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S4_OBJECT);
}

void DMA2_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S5_OBJECT);
}

void DMA2_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S6_OBJECT);
}

void DMA2_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S7_OBJECT);
}

void DMA2_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S0_OBJECT);
}
//##############################################################################

#endif

