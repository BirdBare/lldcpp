//
//
//
//
//



#include "spi_lld.h"

uint32_t LldSpiConfig(
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
	if((spi_object->spi->SR & SPI_SR_BSY) == 0)
	{
		RccResetPeripheral(&spi_object->rcc);

		spi_object->spi_config = 0;

		return 0;
	}

	return 1;
}

//############# POLLED FLAGS SPI CONTROL ########################

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
//######################### END POLLED FLAGS SPI CONTROL ##################

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

//####################### END INTERRUPT CONTROL ##################

//#############SPI SPECIFIC FUNCTIONS###############

uint32_t LldSpiTransmit(
	struct SpiObject *spi_object, 
	void *data_out,
	uint32_t num_data)
{
	uint32_t (*transmit_function[3])(struct SpiObject *spi_object,
		void *data_out, uint32_t num_data) =
			{&LldSpiTransmitPolled, &LldSpiTransmitDma, &LldSpiTransmitInterrupt};

	return transmit_function[spi_object->spi_config->spi_mode](spi_object,data_out,
		num_data);
}

uint32_t LldSpiTransfer(
	struct SpiObject *spi_object, 
	void *data_out,
	void *data_in,
	uint32_t num_data)
{
	uint32_t (*transfer_function[3])(struct SpiObject *spi_object,
		void *data_out, void *data_in, uint32_t num_data) =
		{&LldSpiTransferPolled, &LldSpiTransferDma, &LldSpiTransferInterrupt};

	return transfer_function[spi_object->spi_config->spi_mode](spi_object,data_out,
		data_in,num_data);
}


uint32_t LldSpiReceive(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	uint32_t (*receive_function[3])(struct SpiObject *spi_object,
		void *data_in, uint32_t num_data) =
		{&LldSpiReceivePolled, &LldSpiReceiveDma, &LldSpiReceiveInterrupt};

	return receive_function[spi_object->spi_config->spi_mode](spi_object,data_in,
		num_data);
}

//END



//
// 
//
uint32_t LldSpiPutDataObject(struct SpiObject *spi_object, uint32_t data)
{
	if(spi_object->rx_buffer.buffer_size != 0)
	{
		if((spi_object->spi_config->cr1 & SPI_CR1_DFF) == 0)
		{
			*((uint8_t *)spi_object->rx_buffer.buffer++) = data;
		}
		else
		{
			*((uint16_t *)spi_object->rx_buffer.buffer++) = data;
		}
		//set the data

		return --spi_object->rx_buffer.buffer_size;
		//return how many data left and decrement by one.
	}
	return 0;
	//return 0 data left
}

//
//
//
void LldSpiGetDataDevice(struct SpiObject *spi_object, uint32_t *data)
{
	*data = spi_object->spi->DR;
}

//
// SPI INTERRUPT FUNCTION STORE RECEIVED DATA
//
uint32_t LldSpiGetDataObject(struct SpiObject *spi_object, void *data)
{
	if(spi_object->tx_buffer.buffer_size != 0)
	{
		if((spi_object->spi_config->cr1 & SPI_CR1_DFF) == 0)
		{
			*(uint8_t *)data = *(uint8_t *)spi_object->tx_buffer.buffer++;
		}
		else
		{
			*(uint16_t *)data = *(uint16_t *)spi_object->tx_buffer.buffer++;
		}
		//set the data

		return --spi_object->tx_buffer.buffer_size;
		//return how many data left and decrement by one.
	}
	return 0;
	//zero data left
}

//
//
//
void LldSpiPutDataDevice(struct SpiObject *spi_object, uint32_t data)
{
	spi_object->spi->DR = data;
}
//
// SPI GENERAL INTERRUPT HANDLER
//
ALWAYS_INLINE void GENERAL_SPI_HANDLER(struct SpiObject *spi_object)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	if(LldSpiTransmitReady(spi_object) != 0)
	{	
		uint32_t data = 0, ret;	
		ret = LldSpiGetDataObject(spi_object,&data);
		LldSpiPutDataDevice(spi_object,data);
		
		if(ret == 0)
		{
			if(spi->CRCPR != 0)
			{
				spi->CR1 |= SPI_CR1_CRCNEXT;
				//if crc is enabled then the crc is transfered after the last data.
			}

			LldSpiDisableTxInterrupt(spi_object);
			//if buffer is empty then disable interrupt

			if((spi->CR2 & SPI_CR2_RXNEIE) != 0)
			{
				spi_object->spi_config->callback != 0 ? 
				spi_object->spi_config->callback(spi_object->spi_config->args) : 0;
				//call end of transfer callback if set
			}
			//if rx interrupt is enabled then callback is handled there
		}
	}
//DEAL WITH TX
	
	if(LldSpiReceiveReady(spi_object) != 0)
	{
		uint32_t data, ret;
		LldSpiGetDataDevice(spi_object,&data);
		ret = LldSpiPutDataObject(spi_object,data);

		if(ret == 0)
		{
			LldSpiDisableRxInterrupt(spi_object);
			//if buffer is empty then disable interrupt

			spi_object->spi_config->callback != 0 ? 
				spi_object->spi_config->callback(spi_object->spi_config->args) : 0;
				//call end of transfer callback if set
		}
	}
//DEAL WITH RX 


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
	void (*interrupt)(void *args) =
		SPI1_OBJECT.spi_config->interrupt;
	//get user set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI1_OBJECT);
		//if interrupt is not set then we run the general interrupt
	}
	else
	{
		interrupt(SPI1_OBJECT.spi_config->args);
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
	void (*interrupt)(void *args) =
		SPI2_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI2_OBJECT);
	}
	else
	{
		interrupt(SPI2_OBJECT.spi_config->args);
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
	void (*interrupt)(void *args) =
		SPI3_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI3_OBJECT);
	}
	else
	{
		interrupt(SPI3_OBJECT.spi_config->args);
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
	void (*interrupt)(void *args) =
		SPI4_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI4_OBJECT);
	}
	else
	{
		interrupt(SPI4_OBJECT.spi_config->args);
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
	void (*interrupt)(void *args) =
		SPI5_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI5_OBJECT);
	}
	else
	{
		interrupt(SPI5_OBJECT.spi_config->args);
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
	void (*interrupt)(void *args) =
		SPI6_OBJECT.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI6_OBJECT);
	}
	else
	{
		interrupt(SPI6_OBJECT.spi_config->args);
	}
	//if it is set then we always run it instead of the default
}
#endif





















