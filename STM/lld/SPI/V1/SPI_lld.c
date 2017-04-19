//
//
//
//
//



#include "SPI_lld.h"
#include "RCC_lld.h"

struct SPIxo
	SPI1o = {{0x44,12},0,SPI1},
	SPI2o = {{0x40,14},0,SPI2},
	SPI3o = {{0x40,15},0,SPI3},
	SPI4o = {{0x44,13},0,SPI4},
	SPI5o = {{0x44,20},0,SPI5},
	SPI6o = {{0x44,21},0,SPI6};


int SPI_Config(struct SPIxo *SPIo, int CR1, int CR2, int CRCPR)
{
	volatile SPI_TypeDef * const SPIx = SPIo->SPIx;

	if((SPIx->CR1 & SPI_CR1_SPE) == 0)
	{
		SPIx->CRCPR = CRCPR;
		SPIx->CR2 = CR2;
		SPIx->CR1 = CR1;

		return 0;
	}

	return SPI_CONFIG_ENABLED;


}

int SPI_ResetConfig(struct SPIxo *SPIo)
{
	if((SPIo->SPIx->CR1 & SPI_CR1_SPE) == 0)
	{
		RCC_Reset((struct RCCxo *)SPIo);

		return 0;
	}

	return SPI_CONFIG_ENABLED;
}

void SPI_Enable(struct SPIxo *SPIo)
{
	SPIo->SPIx->CR1 |= SPI_CR1_SPE;
}

int SPI_Disable(struct SPIxo *SPIo)
{
	volatile SPI_TypeDef * const SPIx = SPIo->SPIx;

	if((SPIx->SR & (SPI_SR_BSY | SPI_SR_FTLVL | SPI_SR_FRLVL)) == 0)
	{	
		SPIx->CR1 &= ~SPI_CR1_SPE;

		return 0;
	}
	return SPI_DISABLE_TRANSFER;
}












