//
//
//
//
//



#ifndef SPI_INTERRUPT_HPP
#define SPI_INTERRUPT_HPP

#include "spi_object.hpp"




//******************************************************************************
//
//
//
//******************************************************************************
void SPI_INTERRUPT_INTERRUPT(void *spi_int);

class SpiInterrupt : public SpiObject
{
	void *_tx_data;
	void *_rx_data;

	uint16_t _tx_num_data;
	uint16_t _rx_num_data;
public:

	void SpiPutDataHal(uint32_t data);
	uint32_t SpiGetDataHal(void );
	void SpiPutDataDevice(uint32_t data);
	uint32_t SpiGetDataDevice(void );
	uint32_t SpiTxDecrementNumData(void );
	uint32_t SpiRxDecrementNumData(void );
	uint32_t SpiTransmitReady(void)
	{
		return (_hal.spi->SR & SPI_SR_TXE) && (_hal.spi->CR2 & SPI_CR2_TXEIE);
	}
	uint32_t SpiReceiveReady(void)
	{
		return _hal.spi->SR & SPI_SR_RXNE && (_hal.spi->CR2 & SPI_CR2_RXNEIE);
	}
	void SpiTxDisableInterrupt(void)
	{
		_hal.spi->CR2 &= ~SPI_CR2_TXEIE;
	}
	void SpiRxDisableInterrupt(void)
	{
		_hal.spi->CR2 &= ~SPI_CR2_RXNEIE;
	}

	//write own interrupt functions

	
	void SetInterrupt(void (*interrupt)(void *args), void *interrupt_args)
	{ 
		_interrupt = interrupt; 
		_interrupt_args = interrupt_args; 
	}
	void ResetInterrupt(void)
	{	
		SetInterrupt(&SPI_INTERRUPT_INTERRUPT,this); 
	}
	//call get and set interrupt

	uint32_t Transmit(void *data_out, uint32_t num_data)
	{
		_tx_data = data_out;
		_tx_num_data = num_data;

		PreTransmission();

		_hal.spi->CR1 |= SPI_CR1_SPE | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;

		_hal.spi->CR2 |= SPI_CR2_TXEIE;

		return 0;
	}
	
	uint32_t Transfer(void *data_out,void *data_in, uint16_t num_data)	
	{
		_tx_data = data_out;
		_tx_num_data = num_data;
		_rx_data = data_in;
		_rx_num_data = num_data;

		PreTransmission();

		_hal.spi->CR1 |= SPI_CR1_SPE;

		_hal.spi->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;

		return 0;
	}

	SpiInterrupt(SpiHal &hal)
	: SpiObject(hal), _tx_data(0), _rx_data(0), _tx_num_data(0), _rx_num_data(0)
	{
		_interrupt = &SPI_INTERRUPT_INTERRUPT;
	}

	~SpiInterrupt()
	{
		Deinit();
	}

};














#endif








