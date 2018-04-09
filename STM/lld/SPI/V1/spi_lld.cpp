//
//
//
//
//
//



#include "spi_lld.hpp"


 uint32_t SpiObject::PreTransmission(void)
 { 
		_hal->spi->CR1 = _settings.cr1;
		//reset spi settings for new transfer and get partial data size..
		//and set crc polynomial
 
		if(_settings.crc_polynomial != 0)
		{
			_hal->spi->CRCPR = _settings.crc_polynomial;
			_hal->spi->CR1 |= SPI_CR1_CRCEN;
		}
 
		_hal->spi->CR2 = SPI_CR2_SSOE | SPI_CR2_ERRIE;
		//reset cr2 register to user settings

		_hal->spi->DR; //dummy read

		return 0;
	}




uint32_t SpiObject::Transmit(void *data_out, uint16_t num_data)
{
	data_out = data_out;
	num_data = num_data;

	PreTransmission();

	if(_interrupt != 0)
	{
		_hal->spi->CR2 |= SPI_CR2_TXEIE;
		_hal->owner = this;
	}
	//if interrupt is set then enable it for transfer

	_hal->spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;

	return 0;
}
uint32_t SpiObject::Transfer(
	void *data_out,
	void *data_in,
	uint32_t num_data)
{
	data_out = data_out;
	data_in = data_in;
	num_data = num_data;

	PreTransmission();

	if(_interrupt != 0)
	{
		_hal->spi->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;;
		_hal->owner = this;
	}
	//if interrupt is set then enable it for transfer

	return 0;
}
//Transmit Receive Transfer


uint32_t SpiPolled::Transmit(void *data_out, uint16_t num_data)
{
	SpiObject::Transmit(data_out, num_data);

	GetHal()->spi->CR1 |= SPI_CR1_SPE;

	SPI_DATA_SIZE dff = GetDataSize();
	//get 8 bit or 16 bit data

	do
	{
		if(dff == SPI_DATA_SIZE_8)
		{
			GetHal()->spi->DR = *(uint8_t *)data_out;
			data_out = (void *)((uint32_t)data_out + sizeof(uint8_t));
		}
		else
		{
			GetHal()->spi->DR = *(uint16_t *)data_out;
			data_out = (void *)((uint32_t)data_out + sizeof(uint16_t));
		}
		//decide between 8 bit and 16 bit data;

		do
		{
			NOP;
		} while((GetHal()->spi->SR & SPI_SR_TXE) == 0);
		//wait till buffer is empty

	}while(--num_data != 0);

	if(GetSettings().crc_polynomial != 0)
	{
		GetHal()->spi->CR1 |= SPI_CR1_CRCNEXT;
		//if crc is enabled then the crc is transfered after the last data.
	}

	return 0;
}


uint32_t SpiPolled::Transfer(void *data_out, void *data_in, uint16_t num_data)
{
	SpiObject::Transfer(data_out, data_in, num_data);

	GetHal()->spi->CR1 |= SPI_CR1_SPE;

	SPI_DATA_SIZE dff = GetDataSize();
	//get 8 bit or 16 bit data

	do
	{
		if(dff == SPI_DATA_SIZE_8)
		{
			GetHal()->spi->DR = *(uint8_t *)data_out;
			data_out = (void *)((uint32_t)data_out + sizeof(uint8_t));
		}
		else
		{
			GetHal()->spi->DR = *(uint16_t *)data_out;
			data_out = (void *)((uint32_t)data_out + sizeof(uint16_t));
		}
		//decide between 8 bit and 16 bit data;

		if(num_data == 1  && GetSettings().crc_polynomial != 0)
		{
			GetHal()->spi->CR1 |= SPI_CR1_CRCNEXT;
			//if crc is enabled then the crc is transfered after the last data.
		}

		do
		{
			NOP;
		} while((GetHal()->spi->SR & SPI_SR_RXNE) == 0 && num_data != 0);
		//wait till buffer is not empty

		if(dff == SPI_DATA_SIZE_8)
		{
			*(uint8_t *)data_in = GetHal()->spi->DR;
			data_in = (void *)((uint32_t)data_in + sizeof(uint8_t));
		}
		else
		{
			*(uint16_t *)data_in = GetHal()->spi->DR;
			data_in = (void *)((uint32_t)data_in + sizeof(uint16_t));
		}
		//decide between 8 bit and 16 bit data;
	}while(--num_data != 0);

	return 0;
}




/*

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
*/

#ifdef SPI1
struct SpiHal SPI1_HAL ={
	{&RCC->APB2ENR,
	RCC_APB2ENR_SPI1EN,
	&RCC->APB2RSTR,
	RCC_PERIPHERAL_BUS_APB2},
	{1,
	(IRQn_Type[1]){SPI1_IRQn}},
	SPI1_TX_DMA_CHANNEL,
	SPI1_RX_DMA_CHANNEL,
	SPI1_TX_DMA_HAL,
	SPI1_RX_DMA_HAL,
	SPI1};

#endif
/*
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




















