//
//
//
//
//



#ifndef STM32F767_DMA_H
#define STM32F767_DMA_H

#include "board.h"
#include "rcc_lld.h"
#include "event.h"


//to get DMA Base address. Take DMA_Stream & ~255.

struct DmaObject
{
	const struct RccObject rcc;

	const uint8_t flag_register_offset;
	const uint8_t flag_offset;

	volatile DMA_Stream_TypeDef * const dma_stream;

	volatile struct Mutex *mutex;

	struct EventSource event;
}; 

extern struct DmaObject 
	DMA1_STREAM0_OBJECT,
	DMA1_STREAM1_OBJECT,
	DMA1_STREAM2_OBJECT,
	DMA1_STREAM3_OBJECT,
	DMA1_STREAM4_OBJECT,
	DMA1_STREAM5_OBJECT,
	DMA1_STREAM6_OBJECT,
	DMA1_STREAM7_OBJECT,

	DMA2_STREAM0_OBJECT,
	DMA2_STREAM1_OBJECT,
	DMA2_STREAM2_OBJECT,
	DMA2_STREAM3_OBJECT,
	DMA2_STREAM4_OBJECT,
	DMA2_STREAM6_OBJECT,
	DMA2_STREAM7_OBJECT;

struct DmaConfig
{
	union
	{
		struct
		{
			uint32_t cr; //configuration register
			uint32_t ndtr; //number of data to transfer register
			uint32_t par; //peripheral address register
			uint32_t m0ar; //memory 0 address register
			uint32_t m1ar; //memory 1 address register
			uint32_t fcr; //fifo control register
		};

		struct
		{
			//LSB
			uint32_t :1; // 
			uint32_t dmeie:1; // Direct mode error interrupt enable
			uint32_t teie:1; // Transfer Error Interrupt enable
			uint32_t htie:1; // Half transfer complete enable
			uint32_t tcie:1; // Transfer Complete Interrupt enable
			uint32_t pfctrl:1; // Peripheral Flow Controller select
#define PFCTRL_DMA 0
#define PFCTRL_PERIPHERAL 1

			uint32_t dir:2; //Data transfer Direction
#define DIR_P2M 0
#define DIR_M2P 1
#define DIR_M2M 2

			uint32_t circ:1; //Circular mode enable bit
			uint32_t pinc:1; //Peripheral address increment enable
			uint32_t minc:1; //memory address increment enable
			uint32_t psize:2; //Peripheral data size
			uint32_t msize:2; //Memory data size
#define SIZE_BYTE 0
#define SIZE_HALFWORD 1
#define SIZE_WORD 2

			uint32_t pincos:1; // Peripheral Increment disable bit
			uint32_t pl:2; //priority level
#define PL_LOW 0
#define PL_MED 1
#define PL_HIGH 2
#define PL_VHIGH 3

			uint32_t dbm:1; //Double buffer mode enable
			uint32_t ct:1; // current target for double buffer mode
			uint32_t :1; //
			uint32_t pburst:2; //Peripheral Burst Length
			uint32_t mburst:2; //Memory Burst Length
#define BURST_1 0
#define BURST_4 1
#define BURST_8 2
#define BURST_16 3

			uint32_t chsel:3; //Channel Selection for peripherals
#define CHSEL_0 0
#define CHSEL_1 1
#define CHSEL_2 2
#define CHSEL_3 3
#define CHSEL_4 4
#define CHSEL_5 5
#define CHSEL_6 6
#define CHSEL_7 7

			uint32_t :4;
			//MSB

			//LSB
			uint32_t :32; //holder for Number of data register	
			//MSB

			//LSB
			uint32_t :32; // holder for peripheral address register
			//MSB
			//LSB
			uint32_t :32; //holder for memory 1 address register
			//MSB
			//LSB
			uint32_t :32; //holder for memory 2 address register
			//MSB
			//LSB
			uint32_t fth:2; //fifo threshold size
#define FTH_14
#define FTH_24
#define FTH_34
#define FTH_44

			uint32_t dmdis:1; //direct mode disable
			uint32_t :3; //fifo status bits
			uint32_t :1;
			uint32_t feie:1; //fifo error interrupt enable
			uint32_t :24; 
			//MSB
			//LSB

			//MSB
		};
	};
};

#define DMA_ISR_FEIF 0b0
#define DMA_ISR_DMEIF 0b001
#define DMA_ISR_TEIF 0b0001
#define DMA_ISR_HTIF 0b00001
#define DMA_ISR_TCIF 0b000001



//******************************************************************************
//	
//										 Dma Config
//	
//******************************************************************************
uint32_t DmaConfig(struct DmaObject *dma_stream_object, struct DmaConfig *dma_config);
//##############################################################################

//******************************************************************************
//	
//										 Dma enable/disable
//	
//******************************************************************************

void DmaEnable(struct DmaObject *dma_stream_object);
void DmaDisable(struct DmaObject *dma_stream_object);
//##############################################################################

//******************************************************************************
//	
//										Dma Get/Clear flags 
//	
//******************************************************************************

static uint32_t DmaGetFlags(struct DmaObject *dma_stream_object)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_stream_object->dma_stream;
	//get dma stream

	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma

	return 0b111111 & (*(uint32_t *)((uint32_t)&dma->LIFCR + 
		dma_stream_object->flag_register_offset) >> dma_stream_object->flag_offset);
	//return the flag register value
}

static void DmaClearFlags(struct DmaObject *dma_stream_object, uint32_t flags)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_stream_object->dma_stream;
	//get dma stream

	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma

	*(uint32_t *)((uint32_t)&dma->LIFCR + 
		dma_stream_object->flag_register_offset + 8)  = 
			flags << dma_stream_object->flag_offset;
	//return the flag register value
}
//##############################################################################

#endif
