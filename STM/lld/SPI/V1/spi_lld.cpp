//
//
//
//
//
//



#include "spi_lld.hpp"


 void SpiObject::PreTransmission(void)
 { 
		if(_hal.owner != this)
			BREAK(0);

		_hal.spi->CR1 = _settings.cr1;
		//reset spi settings for new transfer and get partial data size..
 
		_hal.spi->I2SCFGR = 0;

		_hal.spi->CRCPR = _settings.CrcPolynomial();

		if(_settings.CrcPolynomial() != 0)
		{
			_hal.spi->CR1 |= SPI_CR1_CRCEN;
		}
		//and set crc polynomial
 
		_hal.spi->CR2 = SPI_CR2_SSOE | SPI_CR2_ERRIE;
		//reset cr2 register to user settings

		_hal.spi->DR; //dummy read
	}


uint32_t SpiObject::Transmit(void *data_out,uint32_t num_data)
{
	PreTransmission();

	_hal.spi->CR1 |= SPI_CR1_SPE | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;

	uint32_t dff = _settings.DataSize();
	//get 8 bit or 16 bit data

	do
	{
		if(dff == 8)
		{
			_hal.spi->DR = *(uint8_t *)data_out;
			data_out = (void *)((uint32_t)data_out + sizeof(uint8_t));
		}
		else
		{
			_hal.spi->DR = *(uint16_t *)data_out;
			data_out = (void *)((uint32_t)data_out + sizeof(uint16_t));
		}
		//decide between 8 bit and 16 bit data;

		do
		{
			NOP;
		} while((_hal.spi->CR1 & SPI_CR1_SPE)!=0 && (_hal.spi->SR & SPI_SR_TXE)==0);
		//wait till buffer is not empty

	}while((_hal.spi->CR1 & SPI_CR1_SPE) != 0 && --num_data != 0);
	
	if(_hal.spi->CRCPR != 0)
	{
		_hal.spi->CR1 |= SPI_CR1_CRCNEXT;
		//if crc is enabled then the crc is transfered after the last data.
	}

	return num_data;
}
uint32_t SpiObject::Transfer(void *data_out, void *data_in, uint32_t num_data)
{
	PreTransmission();

	_hal.spi->CR1 |= SPI_CR1_SPE;

	uint32_t dff = _settings.DataSize();
	//get 8 bit or 16 bit data

	do
	{
		if(dff == 8)
		{
			_hal.spi->DR = *(uint8_t *)data_out;
			data_out = (void *)((uint32_t)data_out + sizeof(uint8_t));
		}
		else
		{
			_hal.spi->DR = *(uint16_t *)data_out;
			data_out = (void *)((uint32_t)data_out + sizeof(uint16_t));
		}
		//decide between 8 bit and 16 bit data;

		if(num_data == 1 && _hal.spi->CRCPR != 0)
		{
			_hal.spi->CR1 |= SPI_CR1_CRCNEXT;
			//if crc is enabled then the crc is transfered after the last data.
		}

		do
		{
			NOP;
		} while((_hal.spi->CR1 & SPI_CR1_SPE)!=0 && (_hal.spi->SR & SPI_SR_RXNE)==0);
		//wait till buffer is not empty

		if(dff == 8)
		{
			*(uint8_t *)data_in = _hal.spi->DR;
			data_in = (void *)((uint32_t)data_in + sizeof(uint8_t));
		}
		else
		{
			*(uint16_t *)data_in = _hal.spi->DR;
			data_in = (void *)((uint32_t)data_in + sizeof(uint16_t));
		}
		//decide between 8 bit and 16 bit data;
	}while((_hal.spi->CR1 & SPI_CR1_SPE) != 0 && --num_data != 0);

	return num_data;
}




uint32_t SpiInterrupt::SpiTxDecrementNumData(void)
{
	if(_tx_num_data != 0)
	{
		if(_settings.DataSize() == 8)
		{
			_tx_data = (void *)((uint32_t)_tx_data + sizeof(uint8_t));
		}
		else
		{
			_tx_data = (void *)((uint32_t)_tx_data + sizeof(uint16_t));
		}
		//increment the data pointer

		return --_tx_num_data;
	}	

	return 0;
	//return 0 data left
}


uint32_t SpiInterrupt::SpiRxDecrementNumData(void)
{
	if(_rx_num_data != 0)
	{
		if(_settings.DataSize() == 8)
		{
			_rx_data = (void *)((uint32_t)_rx_data + sizeof(uint8_t));
		}
		else
		{
			_rx_data = (void *)((uint32_t)_rx_data + sizeof(uint16_t));
		}
		//increment the data pointer

		return --_rx_num_data;
	}	

	return 0;
	//return 0 data left
}





//
// 
//
void SpiInterrupt::SpiPutDataHal(uint32_t data)
{
		if(_settings.DataSize() == 8)
		{
			*((uint8_t *)_rx_data) = data;
		}
		else
		{
			*((uint16_t *)_rx_data) = data;
		}
		//set the data
}

//
//
//
uint32_t SpiInterrupt::SpiGetDataDevice(void)
{
	return _hal.spi->DR;
}

//
// SPI INTERRUPT FUNCTION STORE RECEIVED DATA
//
uint32_t SpiInterrupt::SpiGetDataHal(void)
{
		if(_settings.DataSize() == 8)
		{
			return *(uint8_t *)_tx_data;
		}
		else
		{
			return *(uint16_t *)_tx_data;
		}
		//get the data
}

//
//
//
void SpiInterrupt::SpiPutDataDevice(uint32_t data)
{
	_hal.spi->DR = data;
}

//
// SPI GENERAL INTERRUPT HANDLER
//
void SPI_INTERRUPT_INTERRUPT(void *spi_int)
{
	SpiInterrupt *spi_object = (SpiInterrupt *)spi_int;

	volatile SPI_TypeDef *spi = spi_object->Hal().spi;
	//get spi

	if(spi_object->SpiReceiveReady() != 0)
	{
		spi_object->SpiPutDataHal(spi_object->SpiGetDataDevice());
		//get data from device and store to user

		if(spi_object->SpiRxDecrementNumData() == 0)
		{
			spi_object->SpiRxDisableInterrupt();
			//if buffer is empty then disable interrupt
			
			if(spi_object->GetCallback() != 0)
			{
				spi_object->GetCallback()(spi_object->GetCallbackArgs());
			}
				//call end of transfer callback if set
		}
		//if num_data is zero then we are finished and need to disable interrupt, ect
	}
	//DEAL WITH RX 

	if(spi_object->SpiTransmitReady() != 0)
	{	
		spi_object->SpiPutDataDevice(spi_object->SpiGetDataHal());
		//get user data and place in data register

		if(spi_object->SpiTxDecrementNumData() == 0)
		{
			if(spi->CRCPR != 0)
			{
				spi->CR1 |= SPI_CR1_CRCNEXT;
				//if crc is enabled then the crc is transfered after the last data.
			}
			
			spi_object->SpiTxDisableInterrupt();
			//if buffer is empty then disable interrupt

			if((spi->CR2 & SPI_CR2_RXNEIE) == 0 && spi_object->GetCallback() != 0)
			{
				spi_object->GetCallback()(spi_object->GetCallbackArgs());
			}
			//call end of transfer callback if set
		}
		//if num_data is zero then we are finished and need to disable interrupt, ect
	}
//DEAL WITH TX
	
	
	if((spi->SR & 0b101110000) != 0)
		BREAK(0);
//DEAL WITH FLAGS
}

void SPI_DMA_INTERRUPT(void *spi_dma)
{
	SpiDma *spi_object = (SpiDma *)spi_dma;
	
	if((spi_object->Hal().spi->SR & 0b101110000) != 0)
		BREAK(0);
}

#ifdef SPI1
struct SpiHal SPI1_HAL ={
	{&RCC->APB2ENR,
	RCC_APB2ENR_SPI1EN,
	&RCC->APB2RSTR,
	RCC_PERIPHERAL_BUS_APB2},
	{1,
	(IRQn_Type[1]){SPI1_IRQn}},
	2, //tx num
	2, //rx num
	(uint8_t [2]){3,3}, //tx channels
	(uint8_t [2]){3,3}, //rx channels
	(DmaHal *[2]){&DMA2S3_HAL,&DMA2S5_HAL}, //tx dmas
	(DmaHal *[2]){&DMA2S0_HAL,&DMA2S2_HAL}, //rx dmas
	SPI1};

extern "C"
{
void SPI1_IRQHandler(void);
}

void SPI1_IRQHandler(void)
{	
	SpiBase *spi_object = SPI1_HAL.owner;

	if(spi_object == 0 || spi_object->GetInterrupt() == 0)
		BREAK(0);

	spi_object->GetInterrupt()(spi_object->GetInterruptArgs());
}
#endif

/*
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




















