//
//
//
//
//
//



#include "spi_object.hpp"

#include "spi_dma.cpp"
#include "spi_interrupt.cpp"

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




















