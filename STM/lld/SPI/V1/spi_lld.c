//
//
//
//
//



#include "spi_lld.h"

uint32_t LldSpiConfigMaster(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config)
{
	spi_object->spi_config = spi_config;
	//set spi config in object

	uint32_t bus_speed = LldClockGetSpeed(spi_object->rcc.peripheral_bus);
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
		//SSOE must be enabled for master to work
	
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




#include "spi_polled_lld.c"
#include "spi_interrupt_lld.c"
#include "spi_dma_lld.c"
//#include "spi_buffer_lld.h" not finished yet


uint32_t LldSpiTxDecrementNumData(struct SpiObject *spi_object)
{
	if(spi_object->tx_buffer.buffer_size != 0)
	{
		if((spi_object->spi->CR1 & SPI_CR1_DFF) == 0)
		{
			(uint8_t *)spi_object->tx_buffer.buffer++;
		}
		else
		{
			(uint16_t *)spi_object->tx_buffer.buffer++;
		}
		//increment the data pointer

		return --spi_object->tx_buffer.buffer_size;
	}	

	return 0;
	//return 0 data left
}

uint32_t LldSpiRxDecrementNumData(struct SpiObject *spi_object)
{
	if(spi_object->rx_buffer.buffer_size != 0)
	{
		if((spi_object->spi->CR1 & SPI_CR1_DFF) == 0)
		{
			(uint8_t *)spi_object->rx_buffer.buffer++;
		}
		else
		{
			(uint16_t *)spi_object->rx_buffer.buffer++;
		}
		//increment the data pointer

		return --spi_object->rx_buffer.buffer_size;
	}	

	return 0;
	//return 0 data left
}






//
// 
//
void LldSpiPutDataObject(struct SpiObject *spi_object, uint32_t data)
{
		if((spi_object->spi->CR1 & SPI_CR1_DFF) == 0)
		{
			*((uint8_t *)spi_object->rx_buffer.buffer) = data;
		}
		else
		{
			*((uint16_t *)spi_object->rx_buffer.buffer) = data;
		}
		//set the data
}

//
//
//
uint32_t LldSpiGetDataDevice(struct SpiObject *spi_object)
{
	return spi_object->spi->DR;
}

//
// SPI INTERRUPT FUNCTION STORE RECEIVED DATA
//
uint32_t LldSpiGetDataObject(struct SpiObject *spi_object)
{
		if((spi_object->spi->CR1 & SPI_CR1_DFF) == 0)
		{
			return *(uint8_t *)spi_object->tx_buffer.buffer;
		}
		else
		{
			return *(uint16_t *)spi_object->tx_buffer.buffer;
		}
		//get the data
}

//
//
//



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
		LldSpiPutDataDevice(spi_object,LldSpiGetDataObject(spi_object));
		//get user data and place in data register

		if(LldSpiTxDecrementNumData(spi_object) == 0)
		{
			if(spi->CRCPR != 0)
			{
				spi->CR1 |= SPI_CR1_CRCNEXT;
				//if crc is enabled then the crc is transfered after the last data.
			}

			LldSpiTxDisableInterrupt(spi_object);
			//if buffer is empty then disable interrupt

			if((spi->CR2 & SPI_CR2_RXNEIE) != 0)
			{
				LldSpiCallCallback(spi_object);
				//call end of transfer callback if set
			}
			//if rx interrupt is enabled then callback is handled there
		}
		//if num_data is zero then we are finished and need to disable interrupt, ect
	}
//DEAL WITH TX
	
	if(LldSpiReceiveReady(spi_object) != 0)
	{
		LldSpiPutDataObject(spi_object,LldSpiGetDataDevice(spi_object));
		//get data from device and store to user

		if(LldSpiRxDecrementNumData(spi_object) == 0)
		{
			LldSpiRxDisableInterrupt(spi_object);
			//if buffer is empty then disable interrupt
			
			LldSpiCallCallback(spi_object);
				//call end of transfer callback if set
		}
		//if num_data is zero then we are finished and need to disable interrupt, ect
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
struct SpiObject SPI1_OBJECT ={{0x44,12,APB2},
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
		interrupt(SPI1_OBJECT.spi_config->interrupt_args);
		//if set then we run user interrupt instead
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI2
struct SpiObject SPI2_OBJECT = {{0x40,14,APB1},
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
		interrupt(SPI2_OBJECT.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI3
struct SpiObject SPI3_OBJECT = {{0x40,15,APB1},
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
		interrupt(SPI3_OBJECT.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI4
struct SpiObject SPI4_OBJECT = {{0x44,13,APB2},
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
		interrupt(SPI4_OBJECT.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI5
struct SpiObject SPI5_OBJECT = {{0x44,20,APB2},
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
		interrupt(SPI5_OBJECT.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI6
struct SpiObject SPI6_OBJECT = {{0x44,21,APB2},
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
		interrupt(SPI6_OBJECT.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif





















