//
//
//
//
//



#include "dma_lld.h"


struct DmaObject
	DMA1S0_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 21,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 0,
		.flag_offset = 0,
		.dma = DMA1_Stream0};

struct DmaObject
DMA1S1_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 21,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 0,
		.flag_offset = 6,
		.dma = DMA1_Stream1};

struct DmaObject
DMA1S2_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 21,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 0,
		.flag_offset = 16,
		.dma = DMA1_Stream2};

struct DmaObject
DMA1S3_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 21,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 0,
		.flag_offset = 22,
		.dma = DMA1_Stream3};

struct DmaObject
DMA1S4_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 21,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 4,
		.flag_offset = 0,
		.dma = DMA1_Stream4};

struct DmaObject
DMA1S5_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 21,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 4,
		.flag_offset = 6,
		.dma = DMA1_Stream5};

struct DmaObject
DMA1S6_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 21,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 4,
		.flag_offset = 16,
		.dma = DMA1_Stream6};

struct DmaObject
DMA1S7_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 21,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 4,
		.flag_offset = 22,
		.dma = DMA1_Stream7};

struct DmaObject
DMA2S0_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 22,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 0,
		.flag_offset = 0,
		.dma = DMA2_Stream0};

struct DmaObject
DMA2S1_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 22,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 0,
		.flag_offset = 6,
		.dma = DMA2_Stream1};

struct DmaObject
DMA2S2_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 22,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 0,
		.flag_offset = 16,
		.dma = DMA2_Stream2};

struct DmaObject
DMA2S3_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 22,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 0,
		.flag_offset = 22,
		.dma = DMA2_Stream3};

struct DmaObject
DMA2S4_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 22,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 4,
		.flag_offset = 0,
		.dma = DMA2_Stream4};

struct DmaObject
DMA2S5_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 22,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 4,
		.flag_offset = 6,
		.dma = DMA2_Stream5};

struct DmaObject
DMA2S6_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 22,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 4,
		.flag_offset = 16,
		.dma = DMA2_Stream6};

struct DmaObject
DMA2S7_OBJECT = {.rcc.reg_offset = 0x30,
		.rcc.bit_offset = 22,
		.rcc.peripheral_bus = AHB,
		.flag_register_offset = 4,
		.flag_offset = 22,
		.dma = DMA2_Stream7};




//******************************************************************************
//	
//									DMA IRQ HANDLERS	 
//	
//******************************************************************************

ALWAYS_INLINE void DMA_STREAM_HANDLER(struct DmaObject *dma_object)
{
	uint32_t flags = LldDmaGetFlags(dma_object);

	if((flags & DMA_ISR_FEIF) != 0)
		BREAK(1);
	if((flags & DMA_ISR_DMEIF) != 0)
		BREAK(2);
	if((flags & DMA_ISR_TEIF) != 0)
		BREAK(3);

		if((((flags & DMA_ISR_TCIF) != 0 && (dma_object->dma->CR & DMA_SxCR_TCIE) != 
		0) || ((flags & DMA_ISR_HTIF) != 0 && (dma_object->dma->CR & DMA_SxCR_HTIE) !=
		0)) && dma_object->callback != 0)
	//if transfer complete flag and interrupt enable bit is set or
	//if half transfer complete flag and interrupt enable bit is set and
	//if callback is set. then we call the callback that was set.
	{
		dma_object->callback(dma_object->args);
		//call callback
	}

	LldDmaClearFlags(dma_object, flags);

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
	DMA_STREAM_HANDLER(&DMA2S0_OBJECT);
}

void DMA2_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S1_OBJECT);
}

void DMA2_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S2_OBJECT);
}

void DMA2_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S3_OBJECT);
}

void DMA2_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S4_OBJECT);
}

void DMA2_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S5_OBJECT);
}

void DMA2_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S6_OBJECT);
}

void DMA2_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S7_OBJECT);
}
//##############################################################################

#endif

