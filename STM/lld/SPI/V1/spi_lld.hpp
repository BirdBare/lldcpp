//
//
//
//
//



#ifndef SPI_LLD_H
#define SPI_LLD_H

#include "board.h"
#include "rcc_lld.h"
#include "nvic_lld.h"
#include "dma_lld.hpp"




//******************************************************************************
//
//
//
//******************************************************************************
struct SpiHal
{
	struct RccHal rcc;
	struct NvicHal nvic;

	uint8_t num_tx_dma;
	uint8_t num_rx_dma;

	uint8_t *tx_dma_channel; 
	uint8_t *rx_dma_channel;

	struct DmaHal **tx_dma;
	struct DmaHal **rx_dma;

	volatile SPI_TypeDef * const spi;

	class SpiBase *owner = 0;

	uint32_t num_owners = 0;
	//number of applications utilizing this hal
};

extern struct SpiHal
	SPI1_HAL,
	SPI2_HAL,
	SPI3_HAL,
	SPI4_HAL,
	SPI5_HAL,
	SPI6_HAL;



//******************************************************************************
//
//
//
//******************************************************************************
enum SPI_CLOCK_PHASE
{
	SPI_CLOCK_PHASE_FIRST = 0,
	SPI_CLOCK_PHASE_SECOND = 1
};

enum SPI_CLOCK_POLARITY
{
	SPI_CLOCK_POLARITY_LOW = 0,
	SPI_CLOCK_POLARITY_HIGH = 1
};

enum SPI_BIT_ORDER
{
	SPI_BIT_ORDER_MSB = 0,
	SPI_BIT_ORDER_LSB = 1
};


struct SpiSettings
{
	SpiSettings& operator=(const SpiSettings &copy)
	{	
		*(uint64_t *)this = *(uint64_t *)&copy; 
		return *this; 
	}

	uint32_t ClockFrequency(void) 
	{
		return clock_frequency;
	}
	SpiSettings& ClockFrequency(uint32_t freq)
	{	
		clock_frequency = freq; 
		return *this;
	}

	SPI_CLOCK_PHASE ClockPhase(void) 
	{
		return clock_phase; 
	}
	SpiSettings& ClockPhase(SPI_CLOCK_PHASE phase) 
	{
		clock_phase = phase; 
		return *this;
	}	
	//get and set clock phase

	bool Master(void) 
	{ 
		return master; 
	}
	SpiSettings& Master(bool state)
	{ 
		master = state; 
		return *this;
	}
	//master initialization

	SPI_CLOCK_POLARITY ClockPolarity(void) 
	{
		return clock_polarity; 
	}
	SpiSettings& ClockPolarity(SPI_CLOCK_POLARITY polarity) 
	{
		clock_polarity = polarity; 
		return *this;
	}
	//get and set polarity
	
	SPI_BIT_ORDER BitOrder(void) 
	{
		return bit_order;
	}
	SpiSettings& BitOrder(SPI_BIT_ORDER order) 
	{
		bit_order = order; 
		return *this;
	}
	//get and set bit order
	
	uint32_t DataSize(void) 
	{
		return 8 << data_size; 
	}
	SpiSettings& DataSize(uint32_t size) 
	{
		data_size = size >> 4; 
		return *this;
	}
	//get and set data size. takes and return number of bytes

	uint32_t CrcPolynomial(void) 
	{
		return crc_polynomial;
	}
	SpiSettings& CrcPolynomial(uint32_t polynomial)
	{ 
		crc_polynomial = polynomial; 
		return *this;
	}


	uint32_t clock_frequency; 
	//spi clock frequency. config calculates be to actual

	union
	{
		struct
		{
			//LSB
			SPI_CLOCK_PHASE clock_phase:1; 
			//When the data is sampled. first or second edge

			SPI_CLOCK_POLARITY clock_polarity:1; 
			//the voltage of the clock when starting
			
			bool master:1;
			//master or slave selection

			uint16_t:4; //padding to get the bits in the right spot for the registers

			SPI_BIT_ORDER bit_order:1; 
			//msb first or lsb first

			uint16_t:3; //padding to get the bits in the right spot for the registers

			uint16_t data_size:1; 
			//size of the data in bits

			uint16_t:4; //padding to get the bits in the right spot for the registers
			//MSB
		};

		uint16_t cr1; //default setting is data size 1 byte using master
		//register like access to settings for actual object use
	};

	uint16_t crc_polynomial; 
	//crc polynomial register
};


//******************************************************************************
//
//
//
//******************************************************************************
class SpiBase
{
protected:
	SpiHal &_hal;

	void (*_interrupt)(void *interrupt_args);
	void *_interrupt_args;

	void (*_callback)(void *callback_args);
	void *_callback_args;

	SpiBase(SpiHal &hal)
	: _hal(hal), _interrupt(0), _interrupt_args(this), 
		_callback(0), _callback_args(0)
	{
		if(hal.num_owners++ == 0)
		{
			NvicEnableHalInterrupt(&hal.nvic);
			RccEnableClock(&hal.rcc);
		}
		//init the object if it has never been connected
	}
	
	~SpiBase()
	{
		if(--_hal.num_owners == 0)
		{
			RccResetPeripheral(&_hal.rcc);
			NvicDisableHalInterrupt(&_hal.nvic);
			RccDisableClock(&_hal.rcc);
		}
		//simple deinit object
	}

public:
	
	SpiHal& Hal(void) 
	{
		return _hal;
	}

	void (*GetCallback(void))(void *args) 
	{ 
		return _callback;
	}
	void * GetCallbackArgs(void) 
	{ 
		return _callback_args; 
	}
	void SetCallback(void (*callback)(void *args), void *callback_args) 
	{	
		_callback = callback; 
		_callback_args = callback_args;
	}
	void ResetCallback(void) 
	{
		SetCallback(0,0); 
	}
	//call get and set Callback

	void (*GetInterrupt(void))(void *args) 
	{ 
		return _interrupt; 
	}
	void * GetInterruptArgs(void) 
	{ 
		return _interrupt_args; 
	}
};



//******************************************************************************
//
//
//
//******************************************************************************
class SpiObject : public SpiBase
{
protected:
	SpiSettings _settings;
	//settings for object

	void PreTransmission(void);

	SpiObject(SpiHal &hal)
	: SpiBase(hal), _settings()
	{}
	
public:

	SpiSettings& Settings(void) 
	{
		return _settings; 
	}
	//Get settings

	bool Status(void)
	{
		return _hal.spi->SR & SPI_SR_BSY;
	}

	void Wait(void)
	{
		while(Status() != 0)
		{
			NOP;
		}
	}

	void Stop(void)
	{
		_hal.spi->CR1 &= ~SPI_CR1_SPE;
	}

	void Init(void)
	{
		if(_hal.owner != 0)
			BREAK(0);

		uint32_t clock_frequency = _settings.clock_frequency;
		//user set clock frequency

		_settings.clock_frequency = RccGetPeripheralSpeed(&_hal.rcc);
		//get bus speed for calculation

		uint32_t br = 0 - 1;
		//counter starts at zero so we need to overflow to zero

		do
		{
			_settings.clock_frequency >>= 1;
			br++;
		} while(clock_frequency < _settings.clock_frequency && br < 7);
		//gets correct register br value and calculates actual clock speed

		_settings.cr1 |= br << 3;
		//config clock frequency

		_hal.owner = this;
	}
	void Deinit(void)
	{
		if(_hal.owner == this)
		{
			Stop();
			_hal.owner = 0;
		}
	}

	uint32_t Transmit(void *data_out, uint32_t num_data);
	
	uint32_t Transfer(void *data_out, void *data_in, uint32_t num_data);
	//transmit and transfer functions
};


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

	uint16_t _tx_num_data = 0;
	uint16_t _rx_num_data = 0;
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
	: SpiObject(hal)
	{
		_interrupt = &SPI_INTERRUPT_INTERRUPT;
	}

	~SpiInterrupt()
	{
		Deinit();
	}

};











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
		if(_tx_dma.Hal().owner == &_tx_dma)
		{
			_tx_dma.Stop();
		}

		if(_rx_dma.Hal().owner == &_rx_dma)
		{
			_rx_dma.Stop();
		}

		SpiObject::Stop();
	}

	SpiDma(SpiHal &hal, uint32_t tx = 0, uint32_t rx = 0)
	: SpiObject(hal),
		_tx_dma(*_hal.tx_dma[tx]),
		_rx_dma(*_hal.rx_dma[rx])
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








