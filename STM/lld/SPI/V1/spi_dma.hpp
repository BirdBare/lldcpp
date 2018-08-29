//
//
//
//
//



#ifndef SPI_DMA_HPP
#define SPI_DMA_HPP

#include "spi_object.hpp"






//******************************************************************************
//
//
//
//******************************************************************************
void SPI_DMA_INTERRUPT(void *spi_dma);

class SpiDma : public SpiObject
{
	DmaInterrupt _tx_dma;
	DmaInterrupt _rx_dma;

public:
	DmaInterrupt& TxDma(void)
	{
		return _tx_dma;
	}

	DmaInterrupt& RxDma(void)
	{
		return _rx_dma;
	}

	void Init(void)
	{
		SpiObject::Init();
		
		_tx_dma.Settings().DataSize(Settings().DataSize());
		//finish setting tx_dma settings
		_tx_dma.Init();

		_rx_dma.Settings().DataSize(Settings().DataSize());
		//finish setting rx_dma settings
		_rx_dma.Init();
	}
	void Deinit(void)
	{
		Stop();
		SpiObject::Deinit();
		_tx_dma.Deinit();
		_rx_dma.Deinit();
	}

	void Stop(void)
	{
		//if(_tx_dma.Hal().owner == &_tx_dma)
		{
			_tx_dma.Stop();
		}

		//if(_rx_dma.Hal().owner == &_rx_dma)
		{
			_rx_dma.Stop();
		}

		SpiObject::Stop();
	}

	SpiDma(SpiHal &hal, uint32_t tx = 0, uint32_t rx = 0)
	: SpiObject(hal), _tx_dma(*_hal.tx_dma[tx]), _rx_dma(*_hal.rx_dma[rx])
	{
		if(tx >= _hal.num_tx_dma || rx >= _hal.num_rx_dma)
			BREAK(0);

		_tx_dma.Settings().Channel(_hal.tx_dma_channel[tx]);
		_rx_dma.Settings().Channel(_hal.rx_dma_channel[rx]);
		//finish setting dma settings

		_interrupt = &SPI_DMA_INTERRUPT;
	}
	
	uint32_t Transmit(void *data_out, uint32_t num_data)
	{
		PreTransmission();

		_hal.spi->CR1 |= SPI_CR1_SPE | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
		
		_tx_dma.SetCallback(_callback,_callback_args);
		_tx_dma.TransferM2P(data_out, (void *)&_hal.spi->DR, num_data);	

		_hal.spi->CR2 |= SPI_CR2_TXDMAEN;

		return 0;
	}

	uint32_t Transfer(void *data_out, void *data_in, uint32_t num_data)
	{
		PreTransmission();

		_hal.spi->CR1 |= SPI_CR1_SPE;
		
		_tx_dma.TransferM2P(data_out, (void *)&_hal.spi->DR, num_data);	

		_rx_dma.SetCallback(_callback,_callback_args);
		_rx_dma.TransferP2M((void *)&_hal.spi->DR, data_in, num_data);	

		_hal.spi->CR2 |= SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN;

		return 0;
	}

	~SpiDma()
	{
		Deinit();
	}
};







#endif








