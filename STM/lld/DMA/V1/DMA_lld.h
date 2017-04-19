//
//
//
//
//



#ifndef STM32F767_DMA_H
#define STM32F767_DMA_H

#include "board.h"
#include "RCC_lld.h"


//to get DMA1 Base address. Take DMAx_Streamx & ~255.

struct DMAx_Streamxo
{
	struct RCCxo RCCo;

	const uint8_t flagregoffset;
	const uint8_t flagflagsoffset;

	volatile DMA_Stream_TypeDef * const DMAx_Streamx;
};

extern struct DMAx_Streamxo
	DMA1_Stream0o,
	DMA1_Stream1o,
	DMA1_Stream2o,
	DMA1_Stream3o,
	DMA1_Stream4o,
	DMA1_Stream5o,
	DMA1_Stream6o,
	DMA1_Stream7o,

	DMA2_Stream0o,
	DMA2_Stream1o,
	DMA2_Stream2o,
	DMA2_Stream3o,
	DMA2_Stream4o,
	DMA2_Stream5o,
	DMA2_Stream6o,
	DMA2_Stream7o;


#define DMA_CONFIG_ENABLED 1
//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t DMA_Config(struct DMAx_Streamxo *DMAx_Streamo, uint32_t CR, 
	uint32_t NDTR, uint32_t PAR, uint32_t M0AR, uint32_t M1AR, uint32_t FCR);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t DMA_ResetConfig(struct DMAx_Streamxo *DMAx_Streamo);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void DMA_Enable(struct DMAx_Streamxo *DMAx_Streamo);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void DMA_Disable(struct DMAx_Streamxo *DMAx_Streamo);




#endif
