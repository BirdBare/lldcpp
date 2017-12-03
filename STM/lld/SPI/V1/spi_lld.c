//
//
//
//
//



#include "spi_lld.h"

// MUST BE DECLARED FOR HAL LIBRARY
uint32_t (*SPI_TRANSMIT_FUNCTIONS[])(struct SpiObject *spi_object) =
	{LldSpiTransmitPolled,LldSpiTransmitInterrupt,LldSpiTransmitDma}; 

uint32_t (*SPI_TRANSFER_FUNCTIONS[])(struct SpiObject *spi_object) =
	{LldSpiTransferPolled,LldSpiTransferInterrupt,LldSpiTransferDma}; 

uint32_t (*SPI_RECEIVE_FUNCTIONS[])(struct SpiObject *spi_object) =
	{0,0,0}; 
// END


uint32_t LldSpiConfig(
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

	spi_config->cr1 |= (br << 3);
	//set SPI_CR1 register values.

	spi_config->cr2 = (spi_config->cr2 & ~SPI_CR2_SSOE) | 
		(!spi_config->multimaster << 2) | SPI_CR2_ERRIE;	
		//deal with multimaster capability and enable errors interrupt always

	return 0;
}

uint32_t LldSpiResetConfig(
	struct SpiObject * const spi_object)
{
	if((spi_object->spi->CR1 & SPI_CR1_SPE) == 0)
	{
		RccResetPeripheral(&spi_object->rcc);

		spi_object->spi_config = 0;

		return 0;
	}

	return 1;
}

uint32_t LldSpiConnect(
	struct SpiObject *spi_object, 
	struct SpiConfig *spi_config)
{
	if(spi_object->spi_config != 0)
	{
		return 1;
	}

	spi_object->spi_config = spi_config;
	return 0;
}

uint32_t LldSpiDisconnect(
	struct SpiObject *spi_object)
{
	spi_object->spi_config = 0;
	return 0;
}



//############# POLLED FLAGS SPI CONTROL ########################

//
// SPI TRANSMIT POLLED
//
uint32_t LldSpiTransmitPolled(
	struct SpiObject *spi_object)
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
			spi->DR = *((uint8_t *)spi_config->data_out++);
		}
		else
		{
			spi->DR = *((uint16_t *)spi_config->data_out++);
		}
		//decide between 8 bit and 16 bit data;

		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_TXE) == 0);
		//wait till buffer is empty
	

	}while(--spi_config->num_data != 0);

	if(crcpr != 0)
	{
		spi->CR1 |= SPI_CR1_CRCNEXT;
		//if crc is enabled then the crc is transfered after the last data.
	}
	
	spi_config->callback != 0 ? 
		spi_config->callback(spi_config->callback_args) : 0;
	//call end of transfer callback if set

	return 0;
}

//
// SPI TRANSFER POLLED
//
uint32_t LldSpiTransferPolled(
	struct SpiObject *spi_object) 
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
			spi->DR = *((uint8_t *)spi_config->data_out++);
		}
		else
		{
			spi->DR = *((uint16_t *)spi_config->data_out++);
		}
		//decide between 8 bit and 16 bit data;
		
		if(spi_config->num_data == 1  && crcpr != 0)
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
			*((uint8_t *)spi_config->data_in++) = spi->DR;
		}
		else
		{
			*((uint16_t *)spi_config->data_in++) = spi->DR;
		}
		//decide between 8 bit and 16 bit data;
	} while(--spi_config->num_data != 0);


	spi_config->callback != 0 ? 
		spi_config->callback(spi_config->callback_args) : 0;
	//call end of transfer callback if set

	return 0;
}



//
// SPI Receive POLLED DOES NOT CURRENTLY WORK DUE TO MASTER RECEIVE ONLY ERROR
//
uint32_t LldSpiReceivePolled(
	struct SpiObject *spi_object) 
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

	if(spi_config->interrupt != 0)
	{
		spi->CR2 |= SPI_CR2_TXEIE;	
	}
	//if interrupt is set then enable it for transfer / receive

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_RXONLY | SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	const uint32_t num_data = spi_config->num_data;

	for(uint32_t counter = 0; counter < num_data; counter++)
	{
		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_RXNE) == 0);
		//wait till buffer is not empty

		if(dff == 0)
		{
			((uint8_t *)spi_config->data_in)[counter] = spi->DR;
		}
		else
		{
			((uint16_t *)spi_config->data_in)[counter] = spi->DR;
		}
		//decide between 8 bit and 16 bit data;

		if(counter == (num_data - 2) && crcpr != 0)
		{
			spi->CR1 |= SPI_CR1_CRCNEXT;
			//if crc is enabled then the crc is transfered after the second last data.
		}
	}
	//receive the data

	spi_config->callback != 0 ? 
		spi_config->callback(spi_config->callback_args) : 0;
	//call end of transfer callback if set

	return 0;
}
//######################### END POLLED FLAGS SPI CONTROL ##################

//
// SPI TRANSFER DMA
//
uint32_t LldSpiTransmitDma(
	struct SpiObject *spi_object) 
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

	DmaConfigNDTR(tx_dma_object, spi_config->num_data);
	DmaConfigPAR(tx_dma_object, (uint32_t *)&spi->DR);
	DmaConfigM0AR(tx_dma_object, spi_config->data_out);
	DmaConfigCR(tx_dma_object, (spi_object->tx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | 1 << 6 | 1);

	spi->CR2 |= SPI_CR2_TXDMAEN | (spi_config->interrupt != 0 ? SPI_CR2_TXEIE : 0);
	//enable dma request for transfer

	return 0;
}

uint32_t LldSpiTransferDma(
	struct SpiObject *spi_object) 
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


	DmaConfigNDTR(tx_dma_object, spi_config->num_data);
	DmaConfigNDTR(rx_dma_object, spi_config->num_data);
	DmaConfigPAR(tx_dma_object, (uint32_t *)&spi->DR);
	DmaConfigPAR(rx_dma_object, (uint32_t *)&spi->DR);
	DmaConfigM0AR(tx_dma_object, spi_config->data_out);
	DmaConfigM0AR(rx_dma_object, spi_config->data_in);
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
	struct SpiObject *spi_object) 
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi
	
	const struct SpiConfig *spi_config = spi_object->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->DR; //dummy read to reset RX flag. prevents overrun error.

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
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
	//clear all interrupt flags 

	DmaConfigNDTR(rx_dma_object, spi_config->num_data);
	DmaConfigPAR(rx_dma_object, (uint32_t *)&spi->DR);
	DmaConfigM0AR(rx_dma_object, spi_config->data_in);
	DmaConfigCR(rx_dma_object, (spi_object->rx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | DMA_SxCR_EN);
	//set dma settings and enable dma for spi

	spi->CR2 |= SPI_CR2_RXDMAEN | (spi_config->interrupt != 0 ? SPI_CR2_RXNEIE : 0);
	//enable dma request for transfer. Transfer starts here.

	return 0;
}
//######################### END DMA SPI CONTROL ##################

//
//	SPI TRANSMIT INTERRUPTS
//
uint32_t LldSpiTransmitInterrupt(
	struct SpiObject *spi_object) 
{
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
	struct SpiObject *spi_object) 
{
	if(spi_object->spi_config->crc_polynomial != 0)
		return 1;
		//special case where crc doesnt work in transfer.

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
//####################### END INTERRUPT CONTROL ##################



//
// SPI GENERAL INTERRUPT FUNCTION GET DATA FOR OUTPUT 
//
void LldSpiPutDataObject(struct SpiObject *spi_object, uint32_t data)
{
	struct SpiConfig *spi_config = spi_object->spi_config;

	if((spi_config->cr1 & SPI_CR1_DFF) == 0)
	{
		*((uint8_t *)spi_config->data_in++) = data;
	}
	else
	{
		*((uint16_t *)spi_config->data_in++) = data;
	}
}

//
//
//
uint32_t LldSpiGetDataDevice(struct SpiObject *spi_object)
{
	volatile SPI_TypeDef *spi = spi_object->spi;

	if(spi_object->spi_config->num_data == 0)
	{
		spi->CR2 &= ~SPI_CR2_RXNEIE;
		//if buffer is full then disable interrupt

		spi_object->spi_config->callback != 0 ? 
			spi_object->spi_config->callback(spi_object->spi_config->callback_args) : 0;
		//call end of transfer callback if set

	}

	return spi->DR;
}

//
// SPI INTERRUPT FUNCTION STORE RECEIVED DATA
//
uint32_t LldSpiGetDataObject(struct SpiObject *spi_object)
{
	struct SpiConfig *spi_config = spi_object->spi_config;

	if((spi_config->cr1 & SPI_CR1_DFF) == 0)
	{
		return *((uint8_t *)spi_config->data_out++);
	}
	else
	{
		return *((uint16_t *)spi_config->data_out++);
	}
}

//
//
//
void LldSpiPutDataDevice(struct SpiObject *spi_object, uint32_t data)
{
	volatile SPI_TypeDef *spi = spi_object->spi;

	spi->DR = data;

	if(--spi_object->spi_config->num_data == 0)
	{
		if(spi->CRCPR != 0)
		{
			spi->CR1 |= SPI_CR1_CRCNEXT;
				//if crc is enabled then the crc is transfered after the last data.
		}

		spi->CR2 &= ~SPI_CR2_TXEIE;
		//if buffer is empty then disable interrupt
		
		if((spi->CR2 & SPI_CR2_RXNEIE) != 0)
		{
			spi_object->spi_config->callback != 0 ? 
			spi_object->spi_config->callback(spi_object->spi_config->callback_args) : 0;
			//call end of transfer callback if set
		}
		//if rx interrupt is enabled then callback is handled there
	}
}


//
// SPI GENERAL INTERRUPT HANDLER
//
ALWAYS_INLINE void GENERAL_SPI_HANDLER(struct SpiObject *spi_object)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	if((spi->CR2 & SPI_CR2_RXNEIE) != 0 && (spi->SR & SPI_SR_RXNE) != 0)
	{
		LldSpiPutDataObject(spi_object, LldSpiGetDataDevice(spi_object));	
	}
//DEAL WITH RX MUST ALWAYS BE BEFORE TX

	if((spi->CR2 & SPI_CR2_TXEIE) != 0 && (spi->SR & SPI_SR_TXE) != 0)
	{	
		LldSpiPutDataDevice(spi_object, LldSpiGetDataObject(spi_object));
	}
//DEAL WITH TX
	
	if((spi->SR & SPI_SR_UDR) != 0)
		BREAK(1);
	if((spi->SR & SPI_SR_CRCERR) != 0)
		BREAK(2);
	if((spi->SR & SPI_SR_MODF) != 0)
		BREAK(3);
	if((spi->SR & SPI_SR_OVR) != 0)
		BREAK(4);
	if((spi->SR & SPI_SR_FRE) != 0)
		BREAK(5);
//DEAL WITH FLAGS
}

#ifdef SPI1
struct SpiObject SPI1_OBJECT ={{0x44,12,3},
SPI1_TX_DMA_CHANNEL,SPI1_RX_DMA_CHANNEL,
SPI1_TX_DMA_OBJECT,SPI1_RX_DMA_OBJECT,
SPI1};

void SPI1_IRQHandler(void)
{	
	void (*interrupt)(struct SpiObject *spi_object) =
		SPI1_OBJECT.spi_config->interrupt;
	//get user set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI1_OBJECT);
		//if interrupt is not set then we run the general interrupt
	}
	else
	{
		interrupt(&SPI1_OBJECT);
		//if set then we run user interrupt instead
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI2
struct SpiObject SPI2_OBJECT = {{0x40,14,2},
SPI2_TX_DMA_CHANNEL,SPI2_RX_DMA_CHANNEL,
SPI2_TX_DMA_OBJECT,SPI2_RX_DMA_OBJECT,
SPI2};

void SPI2_IRQHandler(void)
{	
	void (*interrupt)(struct SpiObject *spi_object) =
		SPI2_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI2_OBJECT);
	}
	else
	{
		interrupt(&SPI2_OBJECT);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI3
struct SpiObject SPI3_OBJECT = {{0x40,15,2},
SPI3_TX_DMA_CHANNEL,SPI3_RX_DMA_CHANNEL,
SPI3_TX_DMA_OBJECT,SPI3_RX_DMA_OBJECT,
SPI3};

void SPI3_IRQHandler(void)
{	
	void (*interrupt)(struct SpiObject *spi_object) =
		SPI3_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI3_OBJECT);
	}
	else
	{
		interrupt(&SPI3_OBJECT);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI4
struct SpiObject SPI4_OBJECT = {{0x44,13,3},
SPI4_TX_DMA_CHANNEL,SPI4_RX_DMA_CHANNEL,
SPI4_TX_DMA_OBJECT,SPI4_RX_DMA_OBJECT,
SPI4};

void SPI4_IRQHandler(void)
{	
	void (*interrupt)(struct SpiObject *spi_object) =
		SPI4_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI4_OBJECT);
	}
	else
	{
		interrupt(&SPI4_OBJECT);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI5
struct SpiObject SPI5_OBJECT = {{0x44,20,3},
SPI5_TX_DMA_CHANNEL,SPI5_RX_DMA_CHANNEL,
SPI5_TX_DMA_OBJECT,SPI5_RX_DMA_OBJECT,
SPI5};

void SPI5_IRQHandler(void)
{	
	void (*interrupt)(struct SpiObject *spi_object) =
		SPI5_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI5_OBJECT);
	}
	else
	{
		interrupt(&SPI5_OBJECT);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI6
struct SpiObject SPI6_OBJECT = {{0x44,21,3},
SPI6_TX_DMA_CHANNEL,SPI6_RX_DMA_CHANNEL,
SPI6_TX_DMA_OBJECT,SPI6_RX_DMA_OBJECT,
SPI6};

void SPI6_IRQHandler(void)
{	
	void (*interrupt)(struct SpiObject *spi_object) =
		SPI6_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI6_OBJECT);
	}
	else
	{
		interrupt(&SPI6_OBJECT);
	}
	//if it is set then we always run it instead of the default
}
#endif





















