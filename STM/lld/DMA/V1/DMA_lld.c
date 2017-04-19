//
//
//
//
//



#include "DMA_lld.h"
//#include "STM32F767_.h"


struct DMAx_Streamxo
	DMA1_Stream0o = {{0x30,21},0,0,DMA1_Stream0},
	DMA1_Stream1o = {{0x30,21},0,6,DMA1_Stream1},
	DMA1_Stream2o = {{0x30,21},0,16,DMA1_Stream2},
	DMA1_Stream3o = {{0x30,21},0,22,DMA1_Stream3},
	DMA1_Stream4o = {{0x30,21},4,0,DMA1_Stream4},
	DMA1_Stream5o = {{0x30,21},4,6,DMA1_Stream5},
	DMA1_Stream6o = {{0x30,21},4,16,DMA1_Stream6},
	DMA1_Stream7o = {{0x30,21},4,22,DMA1_Stream7},

	DMA2_Stream0o = {{0x30,22},0,0,DMA2_Stream0},
	DMA2_Stream1o = {{0x30,22},0,6,DMA2_Stream1},
	DMA2_Stream2o = {{0x30,22},0,16,DMA2_Stream2},
	DMA2_Stream3o = {{0x30,22},0,22,DMA2_Stream3},
	DMA2_Stream4o = {{0x30,22},4,0,DMA2_Stream4},
	DMA2_Stream5o = {{0x30,22},4,6,DMA2_Stream5},
	DMA2_Stream6o = {{0x30,22},4,16,DMA2_Stream6},
	DMA2_Stream7o = {{0x30,22},4,22,DMA2_Stream7};



//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t DMA_Config(struct DMAx_Streamxo *DMAx_Streamo, uint32_t CR, 
	uint32_t NDTR, uint32_t PAR, uint32_t M0AR, uint32_t M1AR, uint32_t FCR)
{
	volatile DMA_Stream_TypeDef * const DMAx_Streamx = DMAx_Streamo->DMAx_Streamx;

	if((DMAx_Streamx->CR & DMA_SxCR_EN) == 0)
	{
		DMAx_Streamx->FCR = FCR;
		DMAx_Streamx->M1AR = M1AR;
		DMAx_Streamx->M0AR = M0AR;
		DMAx_Streamx->PAR = PAR;
		DMAx_Streamx->NDTR = NDTR;
		DMAx_Streamx->CR = CR;
	
		return 0;
	}
	return DMA_CONFIG_ENABLED;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t DMA_ResetConfig(struct DMAx_Streamxo *DMAx_Streamo)
{
	volatile DMA_Stream_TypeDef * const DMAx_Streamx = DMAx_Streamo->DMAx_Streamx;

	if((DMAx_Streamx->CR & DMA_SxCR_EN) == 0)
	{
		uint32_t *clearflagreg = 
			(void *)(((uint32_t)DMAx_Streamx & ~255) + 
				DMAx_Streamo->flagregoffset + 8);
	
		*clearflagreg = 0b111101 << DMAx_Streamo->flagflagsoffset;

		DMAx_Streamx->CR = 0;
		DMAx_Streamx->NDTR = 0;
		DMAx_Streamx->PAR = 0;
		DMAx_Streamx->M0AR = 0;
		DMAx_Streamx->M1AR = 0;
		DMAx_Streamx->FCR = 0;

		return 0;
	}
	return DMA_CONFIG_ENABLED;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
void DMA_Enable(struct DMAx_Streamxo *DMAx_Streamo)
{
	DMAx_Streamo->DMAx_Streamx->CR |= DMA_SxCR_EN;
}


//******************************************************************************
//	
//										 
//	
//******************************************************************************
void DMA_Disable(struct DMAx_Streamxo *DMAx_Streamo)
{
	DMAx_Streamo->DMAx_Streamx->CR &= ~DMA_SxCR_EN;
}











