//
//
//
//
//
//



#include "spi_lld.hpp"
/*
uint32_t LldSpiConfigMaster(
	struct SpiHal * const spi_object,
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

	return 0;
}

uint32_t LldSpiResetConfig(
	struct SpiHal * const spi_object)
{
	RccResetPeripheral(&spi_object->rcc);

	spi_object->spi_config = 0;

	return 1;
}

//zero means available. not zero means busy
uint32_t LldSpiStatus(struct SpiHal * const spi_object)
{
	return (spi_object->spi->SR & SPI_SR_BSY) != 0 ||
		spi_object->tx_num_data != 0 || spi_object->rx_num_data != 0;
}

uint32_t LldSpiStop(struct SpiHal * const spi_object)
{
	spi_object->spi->CR1 = 0;
	//just a hard stop. reset settings in cr spots the current transfer

	return 0;
}


//#include "spi_polled_lld.c"
//#include "spi_interrupt_lld.c"
//#include "spi_dma_lld.c"

uint32_t LldSpiTxDecrementNumData(struct SpiHal *spi_object)
{
	if(spi_object->tx_num_data != 0)
	{
		if((spi_object->spi->CR1 & SPI_CR1_DFF) == 0)
		{
			(uint8_t *)spi_object->tx_data++; 
		}
		else
		{
			(uint16_t *)spi_object->tx_data++;
		}
		//increment the data pointer

		return --spi_object->tx_num_data;
	}	

	return 0;
	//return 0 data left
}

uint32_t LldSpiRxDecrementNumData(struct SpiHal *spi_object)
{
	if(spi_object->rx_num_data != 0)
	{
		if((spi_object->spi->CR1 & SPI_CR1_DFF) == 0)
		{
			(uint8_t *)spi_object->rx_data++;
		}
		else
		{
			(uint16_t *)spi_object->rx_data++;
		}
		//increment the data pointer

		return --spi_object->rx_num_data;
	}	

	return 0;
	//return 0 data left
}






//
// 
//
void LldSpiPutDataHal(struct SpiHal *spi_object, uint32_t data)
{
		if((spi_object->spi->CR1 & SPI_CR1_DFF) == 0)
		{
			*((uint8_t *)spi_object->rx_data) = data;
		}
		else
		{
			*((uint16_t *)spi_object->rx_data) = data;
		}
		//set the data
}

//
//
//
uint32_t LldSpiGetDataDevice(struct SpiHal *spi_object)
{
	return spi_object->spi->DR;
}

//
// SPI INTERRUPT FUNCTION STORE RECEIVED DATA
//
uint32_t LldSpiGetDataHal(struct SpiHal *spi_object)
{
		if((spi_object->spi->CR1 & SPI_CR1_DFF) == 0)
		{
			return *(uint8_t *)spi_object->tx_data;
		}
		else
		{
			return *(uint16_t *)spi_object->tx_data;
		}
		//get the data
}

//
//
//



//
//
//
void LldSpiPutDataDevice(struct SpiHal *spi_object, uint32_t data)
{
	spi_object->spi->DR = data;
}

//
// SPI GENERAL INTERRUPT HANDLER
//
static inline void GENERAL_SPI_HANDLER(struct SpiHal *spi_object)
{
	volatile SPI_TypeDef *spi = spi_object->spi;
	//get spi

	if(LldSpiTransmitReady(spi_object) != 0)
	{	
		LldSpiPutDataDevice(spi_object,LldSpiGetDataHal(spi_object));
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

			if((spi->CR2 & SPI_CR2_RXNEIE) == 0)
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
		LldSpiPutDataHal(spi_object,LldSpiGetDataDevice(spi_object));
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
struct SpiHal SPI1_HAL ={
	{0x44,
	12,
	APB2},
	{1,
	(IRQn_Type[1]){SPI1_IRQn}},
	SPI1_TX_DMA_CHANNEL,
	SPI1_RX_DMA_CHANNEL,
	SPI1_TX_DMA_HAL,
	SPI1_RX_DMA_HAL,
	SPI1};

void SPI1_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI1_HAL.spi_config->interrupt;
	//get user set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI1_HAL);
		//if interrupt is not set then we run the general interrupt
	}
	else
	{
		interrupt(SPI1_HAL.spi_config->interrupt_args);
		//if set then we run user interrupt instead
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI2
struct SpiHal SPI2_HAL ={
	{0x40,
	14,
	APB1},
	{1,
	(IRQn_Type[1]){SPI2_IRQn}},
	SPI2_TX_DMA_CHANNEL,
	SPI2_RX_DMA_CHANNEL,
	SPI2_TX_DMA_HAL,
	SPI2_RX_DMA_HAL,
	SPI2};

void SPI2_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI2_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI2_HAL);
	}
	else
	{
		interrupt(SPI2_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI3
struct SpiHal SPI3_HAL ={
	{0x40,
	15,
	APB1},
	{1,
	(IRQn_Type[1]){SPI3_IRQn}},
	SPI3_TX_DMA_CHANNEL,
	SPI3_RX_DMA_CHANNEL,
	SPI3_TX_DMA_HAL,
	SPI3_RX_DMA_HAL,
	SPI3};

void SPI3_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI3_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI3_HAL);
	}
	else
	{
		interrupt(SPI3_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif
/*
#ifdef SPI4
struct SpiHal SPI4_HAL ={
	.rcc.reg_offset = 0x44,
	.rcc.bit_offset = 13,
	.rcc.nvic.num_irq = 1,
	.rcc.nvic.irq_number = (uint8_t[1]){51},
	.rcc.peripheral_bus = APB2,
	.tx_dma_channel = SPI4_TX_DMA_CHANNEL,
	.rx_dma_channel = SPI4_RX_DMA_CHANNEL,
	.tx_dma_object = SPI4_TX_DMA_HAL,
	.rx_dma_object =SPI4_RX_DMA_HAL,
	.spi = SPI4};

void SPI4_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI4_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI4_HAL);
	}
	else
	{
		interrupt(SPI4_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI5
struct SpiHal SPI5_HAL ={
	.rcc.reg_offset = 0x44,
	.rcc.bit_offset = 20,
	.rcc.nvic.num_irq = 1,
	.rcc.nvic.irq_number = (uint8_t[1]){51},
	.rcc.peripheral_bus = APB2,
	.tx_dma_channel = SPI5_TX_DMA_CHANNEL,
	.rx_dma_channel = SPI5_RX_DMA_CHANNEL,
	.tx_dma_object = SPI5_TX_DMA_HAL,
	.rx_dma_object =SPI5_RX_DMA_HAL,
	.spi = SPI5};

void SPI5_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI5_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI5_HAL);
	}
	else
	{
		interrupt(SPI5_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI6
struct SpiHal SPI6_HAL ={
	.rcc.reg_offset = 0x44,
	.rcc.bit_offset = 21,
	.rcc.nvic.num_irq = 1,
	.rcc.nvic.irq_number = (uint8_t[1]){51},
	.rcc.peripheral_bus = APB2,
	.tx_dma_channel = SPI6_TX_DMA_CHANNEL,
	.rx_dma_channel = SPI6_RX_DMA_CHANNEL,
	.tx_dma_object = SPI6_TX_DMA_HAL,
	.rx_dma_object =SPI6_RX_DMA_HAL,
	.spi = SPI6};

void SPI6_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI6_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI6_HAL);
	}
	else
	{
		interrupt(SPI6_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

*/




















