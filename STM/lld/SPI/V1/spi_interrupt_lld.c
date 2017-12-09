//
//
//
//
//



#include "spi_lld.h"

uint32_t LldSpiConfigMasterInterrupt(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config)
{
	spi_object->spi_config = spi_config;
	//set spi config in object

	uint32_t bus_speed = ClockGetSpeed(spi_object->rcc.peripheral_bus);
	//get bus speed because we will use it in comparison

	uint32_t br = 0 - 1;
	//counter starts at zero. so first count will overflow to zero

	do
	{
		bus_speed >>= 1;
		br++;
	} while(spi_config->clock_frequency < bus_speed && br < 7);
	//calculates actual clock speed and finds the correct register value

	spi_config->clock_frequency = bus_speed;
	//set actual spi speed for user

	spi_config->cr1 |= (br << 3) | SPI_CR1_MSTR;
	//set SPI_CR1 register values.

	spi_config->cr2 = spi_config->cr2 | SPI_CR2_SSOE | SPI_CR2_ERRIE; 
		//deal with multimaster capability and enable errors interrupt always
		//SSOE must be enabled for config to work
	
	return 0;
}



//
//	SPI TRANSMIT INTERRUPTS
//
uint32_t LldSpiTransmitInterrupt(
	struct SpiObject *spi_object, 
	void *data_out,
	uint32_t num_data)
{
	spi_object->tx_buffer.buffer = data_out;
	spi_object->tx_buffer.buffer_size = num_data;
	//set up data inside buffer for interrupt accessing variables

	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer 

	spi->CR2 = spi_config->cr2;
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

	spi_object->tx_buffer.buffer = data_out;
	spi_object->tx_buffer.buffer_size = num_data;
	spi_object->rx_buffer.buffer = data_in;
	spi_object->rx_buffer.buffer_size = num_data;
	//set up data inside buffer for interrupt accessing variables

	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer 

	spi->CR2 = spi_config->cr2;
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


















