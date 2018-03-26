//
//
//
//
//



#include "spi_lld.h"


//
//	SPI TRANSMIT INTERRUPTS
//
uint32_t LldSpiTransmitInterrupt(
	struct SpiObject *spi_object, 
	void *data_out,
	uint32_t num_data)
{
	spi_object->tx_data = data_out;
	spi_object->tx_num_data = num_data;
	//set up data inside buffer for interrupt accessing variables

	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer 

	spi->CR2 = SPI_CR2_SSOE | SPI_CR2_ERRIE;
	//reset cr2 register to user settings

	spi->DR; //dummy read

	spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SPE |
		((spi->CRCPR = spi_config->crc_polynomial) != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	spi->CR2 |= SPI_CR2_TXEIE;
	//enable dma request for transfer. starts the transfer

	return 0;
}


//
//  SPI TRANSFER INTERRUPT
//
uint32_t LldSpiTransferInterrupt(
	struct SpiObject *spi_object, 
	void *data_out,
	void *data_in,
	uint32_t num_data)
{

	spi_object->tx_data = data_out;
	spi_object->tx_num_data = num_data;
	spi_object->rx_data = data_in;
	spi_object->rx_num_data = num_data;
	//set up data inside buffer for interrupt accessing variables

	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer 

	spi->CR2 = SPI_CR2_SSOE | SPI_CR2_ERRIE;
	//reset cr2 register to user settings

	spi->DR; //dummy read to reset RX flag. prevents error.

	spi->CR1 |=  SPI_CR1_SPE | 
		((spi->CRCPR = spi_config->crc_polynomial) != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	spi->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;
	//enable dma request for transfer. starts the transfer

	return 0;
}

// SPI Receive DOES NOT CURRENTLY WORK DUE TO MASTER RECEIVE ONLY ERROR
uint32_t LldSpiReceiveInterrupt(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	return 1;
	}


















