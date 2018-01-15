//
//
//
//
//



#include "spi_lld.h"


//
// SPI TRANSFER DMA
//
uint32_t LldSpiTransmitDma(
	struct SpiObject *spi_object, 
	void *data_out,
	uint32_t num_data)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SPE |
		(crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	struct DmaObject * tx_dma_object = spi_object->tx_dma_object;
	//get tx dma object

	LldDmaClearFlags(tx_dma_object,0b111101);

	LldDmaConfigNDTR(tx_dma_object, num_data);
	LldDmaConfigPAR(tx_dma_object, (uint32_t *)&spi->DR);
	LldDmaConfigM0AR(tx_dma_object, data_out);
	LldDmaConfigCallback(tx_dma_object,spi_config->callback, 
		spi_config->callback_args);
	LldDmaConfigCR(tx_dma_object, (spi_object->tx_dma_channel << 25) | 
		DMA_SxCR_MINC | dff << 2 | dff | 1 << 6 | 1);

	spi->CR2 |= SPI_CR2_TXDMAEN | (spi_config->interrupt != 0 ? SPI_CR2_TXEIE : 0);
	//enable dma request for transfer

	return 0;
}

uint32_t LldSpiTransferDma(
	struct SpiObject *spi_object, 
	void *data_out,
	void *data_in,
	uint32_t num_data)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi
	
	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	struct DmaObject * tx_dma_object = spi_object->tx_dma_object;
	struct DmaObject * rx_dma_object = spi_object->rx_dma_object;
	//get tx & rx dma object

	LldDmaClearFlags(tx_dma_object,0b111101);
	LldDmaClearFlags(rx_dma_object,0b111101);


	LldDmaConfigNDTR(tx_dma_object, num_data);
	LldDmaConfigNDTR(rx_dma_object, num_data);
	LldDmaConfigPAR(tx_dma_object, (uint32_t *)&spi->DR);
	LldDmaConfigPAR(rx_dma_object, (uint32_t *)&spi->DR);
	LldDmaConfigM0AR(tx_dma_object, data_out);
	LldDmaConfigM0AR(rx_dma_object, data_in);
	LldDmaConfigCallback(rx_dma_object,spi_config->callback, 
		spi_config->callback_args);
	LldDmaConfigCR(tx_dma_object, (spi_object->tx_dma_channel << 25) | 
		DMA_SxCR_MINC | dff << 2 | dff | 1 << 6 | 1);
	LldDmaConfigCR(rx_dma_object, (spi_object->rx_dma_channel << 25) | 
		DMA_SxCR_MINC | dff << 2 | dff | 1);

	spi->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN | 
		(spi_config->interrupt != 0 ? SPI_CR2_TXEIE | SPI_CR2_RXNEIE : 0);
	//enable dma request for transfer

	return 0;
}

// SPI Receive DOES NOT CURRENTLY WORK DUE TO MASTER RECEIVE ONLY ERROR
uint32_t LldSpiReceiveDma(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	return 1;
	}
//######################### END DMA SPI CONTROL ##################

















