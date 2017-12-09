//
//
//
//
//



#include "spi_lld.h"

uint32_t LldSpiConfigMasterDma(
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

	DmaClearFlags(tx_dma_object,0b111101);

	DmaConfigNDTR(tx_dma_object, num_data);
	DmaConfigPAR(tx_dma_object, (uint32_t *)&spi->DR);
	DmaConfigM0AR(tx_dma_object, data_out);
	DmaConfigCR(tx_dma_object, (spi_object->tx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | 1 << 6 | 1);

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

	DmaClearFlags(tx_dma_object,0b111101);
	DmaClearFlags(rx_dma_object,0b111101);


	DmaConfigNDTR(tx_dma_object, num_data);
	DmaConfigNDTR(rx_dma_object, num_data);
	DmaConfigPAR(tx_dma_object, (uint32_t *)&spi->DR);
	DmaConfigPAR(rx_dma_object, (uint32_t *)&spi->DR);
	DmaConfigM0AR(tx_dma_object, data_out);
	DmaConfigM0AR(rx_dma_object, data_in);
	DmaConfigCR(tx_dma_object, (spi_object->tx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | 1 << 6 | 1);
	DmaConfigCR(rx_dma_object, (spi_object->rx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | 1);

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

















