//
//
//
//
//



#include "spi_lld.h"

#ifdef SPI1
struct SpiObject SPI1_OBJECT ={{0x44,12,3},
SPI1_TX_DMA_CHANNEL,SPI1_RX_DMA_CHANNEL,
SPI1_TX_DMA_OBJECT,SPI1_RX_DMA_OBJECT,
SPI1};
#endif

#ifdef SPI2
struct SpiObject SPI2_OBJECT = {{0x40,14,2},
SPI2_TX_DMA_CHANNEL,SPI2_RX_DMA_CHANNEL,
SPI2_TX_DMA_OBJECT,SPI2_RX_DMA_OBJECT,
SPI2};
#endif

#ifdef SPI3
struct SpiObject SPI3_OBJECT = {{0x40,15,2},
SPI3_TX_DMA_CHANNEL,SPI3_RX_DMA_CHANNEL,
SPI3_TX_DMA_OBJECT,SPI3_RX_DMA_OBJECT,
SPI3};
#endif

#ifdef SPI4
struct SpiObject SPI4_OBJECT = {{0x44,13,3},
SPI4_TX_DMA_CHANNEL,SPI4_RX_DMA_CHANNEL,
SPI4_TX_DMA_OBJECT,SPI4_RX_DMA_OBJECT,
SPI4};
#endif

#ifdef SPI5
struct SpiObject SPI5_OBJECT = {{0x44,20,3},
SPI5_TX_DMA_CHANNEL,SPI5_RX_DMA_CHANNEL,
SPI5_TX_DMA_OBJECT,SPI5_RX_DMA_OBJECT,
SPI5};
#endif

#ifdef SPI6
struct SpiObject SPI6_OBJECT = {{0x44,21,3},
SPI6_TX_DMA_CHANNEL,SPI6_RX_DMA_CHANNEL,
SPI6_TX_DMA_OBJECT,SPI6_RX_DMA_OBJECT,
SPI6};
#endif

uint32_t SpiConfigMaster(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config)
{
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

	spi_object->spi->CRCPR = spi_config->crc_polynomial;
	//set crc polynomial register

	spi_config->cr1 |= (br << 3) | SPI_CR1_MSTR;
	//set SPI_CR1 register values.

	spi_object->spi_config = spi_config;
	//set pointer for spi actions

	return 0;
}

uint32_t SpiResetConfig(
	const struct SpiObject * const spi_object)
{
	if((spi_object->spi->CR1 & SPI_CR1_SPE) == 0)
	{
		RccResetPeripheral(&spi_object->rcc);

		return 0;
	}

	return 1;
}


//############# POLLED FLAGS SPI CONTROL ########################

//
// SPI TRANSMIT POLLED
//
uint32_t SpiTransmitPolled(
	struct SpiObject *spi_object, 
	uint32_t num_data, 
	void *data_out)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	const uint32_t crcpr = spi->CRCPR;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SPE |
		(crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	for(uint32_t counter = 0; counter < num_data; counter++)
	{
		if(dff == 0)
		{
			spi->DR = ((uint8_t *)data_out)[counter];
		}
		else
		{
			spi->DR = ((uint16_t *)data_out)[counter];
		}
		//decide between 8 bit and 16 bit data;

		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_TXE) == 0);
		//wait till buffer is empty
	}
	//transmit the data

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
uint32_t SpiTransferPolled(
	struct SpiObject *spi_object, 
	uint32_t num_data, 
	void *data_out,
	void *data_in)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi
	
	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	for(uint32_t counter = 0; counter < num_data; counter++)
	{
		if(dff == 0)
		{
			spi->DR = ((uint8_t *)data_out)[counter];
		}
		else
		{
			spi->DR = ((uint16_t *)data_out)[counter];
		}
		//decide between 8 bit and 16 bit data;
		
		if(counter == (num_data - 1) && crcpr != 0)
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
			((uint8_t *)data_in)[counter] = spi->DR;
		}
		else
		{
			((uint16_t *)data_in)[counter] = spi->DR;
		}
		//decide between 8 bit and 16 bit data;
	}
	//transmit and receive the data

	return 0;
}



//
// SPI Receive POLLED
//
uint32_t SpiReceivePolled(
	struct SpiObject *spi_object, 
	uint32_t num_data, 
	void *data_in)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_RXONLY | SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	for(uint32_t counter = 0; counter < num_data; counter++)
	{
		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_RXNE) == 0);
		//wait till buffer is not empty

		if(dff == 0)
		{
			((uint8_t *)data_in)[counter] = spi->DR;
		}
		else
		{
			((uint16_t *)data_in)[counter] = spi->DR;
		}
		//decide between 8 bit and 16 bit data;

		if(counter == (num_data - 2) && crcpr != 0)
		{
			spi->CR1 |= SPI_CR1_CRCNEXT;
			//if crc is enabled then the crc is transfered after the second last data.
		}
	}
	//receive the data

	return 0;
}
//######################### END POLLED FLAGS SPI CONTROL ##################

//
// SPI TRANSFER DMA
//
uint32_t SpiTransmitDma(
	struct SpiObject *spi_object, 
	uint32_t num_data, 
	void *data_out)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	const uint32_t crcpr = spi->CRCPR;
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

	spi->CR2 |= SPI_CR2_TXDMAEN;
	//enable dma request for transfer

	return 0;
}

uint32_t SpiTransferDma(
	struct SpiObject *spi_object, 
	uint32_t num_data, 
	void *data_out,
	void *data_in)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi
	
	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
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

	spi->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;
	//enable dma request for transfer

	return 0;
}

uint32_t SpiReceiveDma(
	struct SpiObject *spi_object, 
	uint32_t num_data, 
	void *data_in)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi
	
	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->DR; //dummy read to reset RX flag. prevents overrun error.

	const uint32_t crcpr = spi->CRCPR;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_RXONLY | SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	struct DmaObject * rx_dma_object = spi_object->rx_dma_object;
	//get rx dma object

	DmaClearFlags(rx_dma_object,0b111101);

	DmaConfigNDTR(rx_dma_object, num_data);
	DmaConfigPAR(rx_dma_object, (uint32_t *)&spi->DR);
	DmaConfigM0AR(rx_dma_object, data_in);
	DmaConfigCR(rx_dma_object, (spi_object->rx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | 1);

	spi->CR2 |= SPI_CR2_RXDMAEN;
	//enable dma request for transfer

	return 0;
}
//######################### END DMA SPI CONTROL ##################

//
//	SPI TRANSFER INTERRUPTS
//
uint32_t SpiTransmitInterrupt(
	struct SpiObject *spi_object, 
	uint32_t num_data, 
	void *data_out)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	const uint32_t crcpr = spi->CRCPR;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SPE |
		(crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size


	spi->CR2 |= SPI_CR2_TXEIE;
	//enable dma request for transfer

	return 0;
}







//####################### END DMA INTERRUPT CONTROL ##################






















