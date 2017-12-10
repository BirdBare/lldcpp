//
//
//
//
//



#include "spi_lld.h"

//
// SPI TRANSMIT POLLED
//
uint32_t LldSpiTransmitPolled(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	if(spi_config->interrupt != 0)
	{
		spi->CR2 |= SPI_CR2_TXEIE;	
	}
	//if interrupt is set then enable it for transfer

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

	do
	{
		if(dff == 0)
		{
			spi->DR = *((uint8_t *)data_out++);
		}
		else
		{
			spi->DR = *((uint16_t *)data_out++);
		}
		//decide between 8 bit and 16 bit data;

		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_TXE) == 0);
		//wait till buffer is empty
	

	}while(--num_data != 0);

	if(crcpr != 0)
	{
		spi->CR1 |= SPI_CR1_CRCNEXT;
		//if crc is enabled then the crc is transfered after the last data.
	}
	
	return 0;
}

//
// SPI TRANSFER POLLED
//
uint32_t LldSpiTransferPolled(
	struct SpiObject *spi_object, 
	void *data_out,
	void *data_in,
	uint32_t num_data)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi
	
	struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	if(spi_config->interrupt != 0)
	{
		spi->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;	
	}
	//if interrupt is set then enable it for transfer / receive

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	do
	{
		if(dff == 0)
		{
			spi->DR = *((uint8_t *)data_out++);
		}
		else
		{
			spi->DR = *((uint16_t *)data_out++);
		}
		//decide between 8 bit and 16 bit data;
		
		if(num_data == 1  && crcpr != 0)
		{
			spi->CR1 |= SPI_CR1_CRCNEXT;
			//if crc is enabled then the crc is transfered after the last data.
		}

		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_RXNE) == 0);
		//wait till buffer is not empty

		if(dff == 0)
		{
			*((uint8_t *)data_in++) = spi->DR;
		}
		else
		{
			*((uint16_t *)data_in++) = spi->DR;
		}
		//decide between 8 bit and 16 bit data;
	} while(--num_data != 0);

	return 0;
}



//
// SPI Receive POLLED DOES NOT CURRENTLY WORK DUE TO MASTER RECEIVE ONLY ERROR
//
uint32_t LldSpiReceivePolled(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
		return 1;
}















