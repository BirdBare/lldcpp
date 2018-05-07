//
//
//
//
//
//



#include "spi_dma.hpp"


void SPI_DMA_INTERRUPT(void *spi_dma)
{
	SpiDma *spi_object = (SpiDma *)spi_dma;
	
	if((spi_object->Hal().spi->SR & 0b101110000) != 0)
		BREAK(0);
}
















