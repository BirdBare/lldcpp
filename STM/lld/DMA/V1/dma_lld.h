//
//
//
//
//



#ifndef STM32F767_DMA_H
#define STM32F767_DMA_H

#include "board.h"
#include "rcc_lld.h"

//to get DMA Base address. Take DMA_Stream & ~255.

struct DmaObject
{
	const struct RccObject rcc;

	const uint8_t flag_register_offset;
	const uint8_t flag_offset;

	volatile DMA_Stream_TypeDef * const dma;

	struct DmaConfig *dma_config;
}; 

extern struct DmaObject 
	DMA1S0_OBJECT,
	DMA1S1_OBJECT,
	DMA1S2_OBJECT,
	DMA1S3_OBJECT,
	DMA1S4_OBJECT,
	DMA1S5_OBJECT,
	DMA1S6_OBJECT,
	DMA1S7_OBJECT,

	DMA2S0_OBJECT,
	DMA2S1_OBJECT,
	DMA2S2_OBJECT,
	DMA2S3_OBJECT,
	DMA2S4_OBJECT,
	DMA2S6_OBJECT,
	DMA2S7_OBJECT;

struct DmaConfig
{
	void * par; //P2M and M2M SOURCE
	void * m0ar; //M2P SOURCE
	void * m1ar; //used in doubel buffer mode

	uint16_t num_data;

	union
	{
		uint16_t fcr;

		struct
		{
		//LSB
			uint16_t fifo_threshold:2;
			uint16_t:14;
		//MSB
		};
	};

	union
	{
		uint32_t cr; //configuration register

		struct
		{
			//LSB	
				uint32_t:5;
				uint32_t peripheral_controller:1;
				uint32_t mode:2;
				uint32_t circular:1;
				uint32_t peripheral_increment:1;
				uint32_t memory_increment:1;
				uint32_t peripheral_data_size:2;
				uint32_t memory_data_size:2;
				uint32_t:1;
				uint32_t priority:2;
				uint32_t double_buffer:1;
				uint32_t :2;
				uint32_t peripheral_burst_size:2;
				uint32_t memory_burst_size:2;
				uint32_t channel:3;
				uint32_t:4;
			//MSB
		};
	};

 void (*callback)(void *callback_args);
 void * callback_args;
}


LldDmaConfig(struct DmaObject *dma_object,
	struct DmaConfig *dma_config);


//two different configuration modes for the dma.
ALWAYS_INLINE void DmaConfigCR(const struct DmaObject const *dma_object,
	const uint32_t cr)
{
	dma_object->dma->CR = cr;
}

ALWAYS_INLINE void DmaConfigNDTR(const struct DmaObject const *dma_object,
	const uint32_t number_data_to_transfer)
{
	dma_object->dma->NDTR = number_data_to_transfer;
}

ALWAYS_INLINE void DmaConfigPAR(const struct DmaObject const *dma_object,
	const void* peripheral_address)
{
	dma_object->dma->PAR = (uint32_t)peripheral_address;
}

ALWAYS_INLINE void DmaConfigM0AR(const struct DmaObject const *dma_object,
	const void* memory_address)
{
	dma_object->dma->M0AR = (uint32_t)memory_address;
}

ALWAYS_INLINE void DmaConfigM1AR(const struct DmaObject const *dma_object,
	const void* memory_address)
{
	dma_object->dma->M1AR = (uint32_t)memory_address;
}

ALWAYS_INLINE void DmaConfigFCR(const struct DmaObject const *dma_object,
	const uint32_t fcr)
{
	dma_object->dma->FCR = fcr;
}

#define DMA_ISR_FEIF 0b0
#define DMA_ISR_DMEIF 0b001
#define DMA_ISR_TEIF 0b0001
#define DMA_ISR_HTIF 0b00001
#define DMA_ISR_TCIF 0b000001



//******************************************************************************
//	
//										 Dma enable/disable
//	
//******************************************************************************

void DmaEnable(struct DmaObject *dma_object);
void DmaDisable(struct DmaObject *dma_object);
//##############################################################################

//******************************************************************************
//	
//										Dma Get/Clear flags 
//	
//******************************************************************************

static uint32_t DmaGetFlags(struct DmaObject *dma_object)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_object->dma;
	//get dma stream

	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma


	uint32_t *flag_register = (uint32_t *)((uint32_t)&dma->LISR + 
		dma_object->flag_register_offset);


	return 0b111101 & (*flag_register >> dma_object->flag_offset);
	//return the flag register value
}

static void DmaClearFlags(struct DmaObject *dma_object, uint32_t flags)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_object->dma;
	//get dma stream

	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma

	uint32_t *flag_register = (uint32_t *)((uint32_t)&dma->LIFCR + 
		dma_object->flag_register_offset);

		*flag_register = (0b111101 & flags) << dma_object->flag_offset;
	//return the flag register value
}
//##############################################################################

#endif
