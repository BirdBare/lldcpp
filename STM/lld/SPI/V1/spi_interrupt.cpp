//
//
//
//
//
//



#include "spi_interrupt.hpp"



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
			
			if(spi_object->Callback() != 0)
			{
				spi_object->Callback()(spi_object->CallbackArgs());
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

			if((spi->CR2 & SPI_CR2_RXNEIE) == 0 && spi_object->Callback() != 0)
			{
				spi_object->Callback()(spi_object->CallbackArgs());
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











