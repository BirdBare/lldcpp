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

	spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = SPI_CR2_SSOE | SPI_CR2_ERRIE;
	//reset cr2 register to user settings

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SPE |
		(crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	struct DmaConfig dma_config = 
	{ .from_address = data_out,
		.to_address = (void *)&spi->DR,
		.num_data = num_data,
		.callback = spi_config->callback,
		.args = spi_config->callback_args,
		.data_size = spi_config->frame_format,
		.priority = 0b11,
		.channel = spi_object->tx_dma_channel};

	LldDmaStartM2P(spi_object->tx_dma_object,&dma_config);

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

	spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = SPI_CR2_SSOE | SPI_CR2_ERRIE;
	//reset cr2 register to user settings

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	struct DmaConfig dma_config = 
	{ .from_address = data_out,
		.to_address = (void *)&spi->DR,
		.num_data = num_data,
		.data_size = spi_config->frame_format,
		.priority = 0b11,
		.channel = spi_object->tx_dma_channel};

	LldDmaStartM2P(spi_object->tx_dma_object,&dma_config);
	//config for tx

	dma_config.from_address = (void *)&spi->DR;
	dma_config.to_address = data_in;
	dma_config.callback = spi_config->callback;
	dma_config.args = spi_config->callback_args;
	dma_config.channel = spi_object->rx_dma_channel;

	LldDmaStartP2M(spi_object->rx_dma_object,&dma_config);
	//config for rx

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

















