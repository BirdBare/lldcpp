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

//******************************************************************************
//
//
//
//******************************************************************************
class SpiObject;

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

	class SpiObject *owner = 0;
	//object that is using or previously used spi

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
struct SpiSettings
{
	SpiSettings& operator=(const SpiSettings &copy)
	{	*(uint64_t *)this = *(uint64_t *)&copy; return *this; }

	inline uint32_t ClockFrequency(void) {return clock_frequency;}
	inline SpiSettings& ClockFrequency(uint32_t freq)
	{	clock_frequency = freq; return *this;}

	inline SPI_CLOCK_PHASE ClockPhase(void) {return clock_phase; }
	inline SpiSettings& ClockPhase(SPI_CLOCK_PHASE phase) 
	{clock_phase = phase; return *this;}
	//get and set clock phase

	inline bool Master(void) { return master; }
	inline SpiSettings& Master(bool state)
	{ master = state; return *this;}
	//master initialization

	inline SPI_CLOCK_POLARITY ClockPolarity(void) 
	{return clock_polarity; }
	inline SpiSettings& ClockPolarity(SPI_CLOCK_POLARITY polarity) 
	{clock_polarity = polarity; return *this;}
	//get and set polarity
	
	inline SPI_BIT_ORDER BitOrder(void) {return bit_order;}
	inline SpiSettings& BitOrder(SPI_BIT_ORDER order) 
	{bit_order = order; return *this;}
	//get and set bit order
	
	inline uint32_t DataSize(void) {return 1 << data_size; }
	inline SpiSettings& DataSize(uint32_t size) 
	{data_size = size >> 1; return *this;}
	//get and set data size. takes and return number of bytes

	inline uint32_t CrcPolynomial(void) {return crc_polynomial;}
	inline SpiSettings& CrcPolynomial(uint32_t polynomial)
	{ crc_polynomial = polynomial; return *this;}


	uint32_t clock_frequency = 0; 
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

			uint32_t data_size:1; 
			//size of the data in bits

			uint16_t:4; //padding to get the bits in the right spot for the registers
			//MSB
		};

		uint16_t cr1 = true << 2; //default setting is data size 1 byte using master
		//register like access to settings for actual object use
	};

	uint16_t crc_polynomial = 0; 
	//crc polynomial register
};




//******************************************************************************
//
//
//
//******************************************************************************
class SpiObject
{
protected:
	SpiHal &_hal;
/*
	void *_interrupt_args = 0; //arguments for interrupt
	void (*_interrupt)(void *args) = 0; //respective spi is argument
																 //replaces default interrupt
																 //if not used then must be 0
*/

	SpiSettings _settings;
	//settings for object

	void PreTransmission(void);

public:
	inline SpiHal& Hal(void) {return _hal;}

	inline SpiSettings& Settings(void) {return _settings; }
	//Get settings

		inline uint32_t Receive(void *data_in, uint16_t num_data)
	{
		data_in = data_in;
		num_data = num_data;

		return 1;
	}
	//Transmit Receive Transfer

	SpiObject(SpiHal &hal,const SpiSettings &settings)
	: _hal(hal)
	{
		_settings = settings;

		uint32_t clock_frequency = _settings.clock_frequency;
		//user set clock frequency

		_settings.clock_frequency = RccGetPeripheralSpeed(&hal.rcc);
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

		
		if(hal.num_owners++ == 0)
		{
			NvicEnableHalInterrupt(&hal.nvic);
			RccEnableClock(&hal.rcc);
		}
		//init the object if it has never been connected
	}
	//constructor for spi object
	
	~SpiObject()
	{
		if(--_hal.num_owners == 0)
		{
			RccResetPeripheral(&_hal.rcc);
			NvicDisableHalInterrupt(&_hal.nvic);
			RccDisableClock(&_hal.rcc);
		}
		//simple deinit object
	}
	//destructor for spi object

};

class SpiPolled : public SpiObject
{
	uint32_t _num_data = 0;
public:
	inline uint32_t Status(void) { return _num_data; }
	//spi status

	inline SpiPolled& Stop(void) { _num_data = 0; return *this; }
	//stop transfer

	uint32_t Transmit(void *data_out, uint16_t num_data);
	uint32_t Transfer(void *data_out, void *data_in, uint16_t num_data);
	//transmit and transfer functions

	SpiPolled(SpiHal &hal, const SpiSettings &settings = {})
	: SpiObject(hal, settings)
	{}
};



class SpiInterrupt : public SpiObject
{
	void (*_interrupt)(void *interrupt_args) = 0;
	void *_interrupt_args = 0;

	void (*_callback)(void *callback_args) = 0;
	void *_callback_args = 0;

	void *_tx_data;
	void *_rx_data;

	uint16_t _tx_num_data = 0;
	uint16_t _rx_num_data = 0;
public:
	inline uint32_t Status(void) {return _tx_num_data | _rx_num_data;} 
	//spi status

	inline SpiInterrupt& Stop(void) 
	{ _tx_num_data = 0; _rx_num_data = 0; return *this; }
	//stop transfer

	void SpiPutDataHal(uint32_t data);
	uint32_t SpiGetDataHal(void );
	void SpiPutDataDevice(uint32_t data);
	uint32_t SpiGetDataDevice(void );
	uint32_t SpiTxDecrementNumData(void );
	uint32_t SpiRxDecrementNumData(void );
	inline uint32_t SpiTransmitReady(void)
	{
		return (_hal.spi->SR & SPI_SR_TXE) && 
			(_hal.spi->CR2 & SPI_CR2_TXEIE);
	}
	inline uint32_t SpiReceiveReady(void)
	{
		return _hal.spi->SR & SPI_SR_RXNE &&
			(_hal.spi->CR2 & SPI_CR2_RXNEIE);
	}
	inline void SpiTxDisableInterrupt(void)
	{
		_hal.spi->CR2 &= ~SPI_CR2_TXEIE;
	}
	inline void SpiRxDisableInterrupt(void)
	{
		_hal.spi->CR2 &= ~SPI_CR2_RXNEIE;
	}

	//write own interrupt functions

	inline void (*GetInterrupt(void))(void *args) { return _interrupt; }
	inline void * GetInterruptArgs(void) { return _interrupt_args; }
	inline void SetInterrupt(
		void (*interrupt)(void *args), 
		void *interrupt_args)
	{ _interrupt = interrupt; _interrupt_args = interrupt_args; }
	inline void ResetInterrupt(void)
	{	SetInterrupt(0,0); }
	//call get and set interrupt

	inline void (*GetCallback(void))(void *args) { return _callback;}
	inline void * GetCallbackArgs(void) { return _callback_args; }
	inline void SetCallback(
		void (*callback)(void *args), 
		void *callback_args) 
	{_callback = callback; _callback_args = callback_args;}
	inline void ResetCallback(void) {SetCallback(0,0); }
	//call get and set Callback

	uint32_t Transmit(void *data_out, uint16_t num_data)
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

	SpiInterrupt(SpiHal &hal, const SpiSettings &settings = {})
	: SpiObject(hal, settings)
	{}

};













class SpiDma : public SpiObject
{
	void (*_callback)(void *args) = 0;
	void *_callback_args = 0;

	DmaInterrupt _tx_dma;
	DmaInterrupt _rx_dma;
public:
	inline uint32_t Status(void) 
	{
		return 
			(_tx_dma.Hal().owner == &_tx_dma ? _tx_dma.Status() : 0)
			|
			(_rx_dma.Hal().owner == &_rx_dma ? _rx_dma.Status() : 0);
	}

	inline SpiDma& Stop(void)
	{
		if(_tx_dma.Hal().owner == &_tx_dma)
		{
			_tx_dma.Hal().dma->CR &= ~DMA_SxCR_EN;
		}

		if(_rx_dma.Hal().owner == &_rx_dma)
		{
			_rx_dma.Hal().dma->CR &= ~DMA_SxCR_EN;
		}

		return *this;
	}

	inline void (*GetCallback(void))(void *args) { return _callback;}
	inline void * GetCallbackArgs(void) { return _callback_args; }
	inline void SetCallback(
		void (*callback)(void *args), 
		void *callback_args) 
	{_callback = callback; _callback_args = callback_args;}
	inline void ResetCallback(void) {SetCallback(0,0); }
	//call get and set Callback

	SpiDma(SpiHal &hal, const SpiSettings &settings = {})
	: SpiObject(hal,settings),
		_tx_dma(*_hal.tx_dma[_hal.num_owners % _hal.num_tx_dma]),
		_rx_dma(*_hal.rx_dma[_hal.num_owners % _hal.num_rx_dma])
	{
		_tx_dma.Settings().DataSize(
			Settings().DataSize()).Priority(DMA_PRIORITY_VHIGH).Channel(
			_hal.tx_dma_channel[_hal.num_owners & _hal.num_tx_dma]);
		//finish setting tx_dma settings

		_rx_dma.Settings().DataSize(
			Settings().DataSize()).Priority(DMA_PRIORITY_VHIGH).Channel(
			_hal.rx_dma_channel[_hal.num_owners & _hal.num_rx_dma]);
		//finish setting rx_dma settings
	}
	
	uint32_t Transmit(void *data_out, uint32_t num_data)
	{
		if(_tx_dma.Status() != 0)
			return 1;
		
		PreTransmission();

		_hal.spi->CR1 |= SPI_CR1_SPE | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
		
		_tx_dma.SetCallback(_callback,_callback_args);
		_tx_dma.TransferM2P(data_out, (void *)&_hal.spi->DR, num_data);	

		_hal.spi->CR2 |= SPI_CR2_TXDMAEN;

		return 0;
	}

	uint32_t Transfer(void *data_out, void *data_in, uint32_t num_data)
	{
		if((_tx_dma.Status() | _rx_dma.Status()) != 0)
			return 1;
		
		PreTransmission();

		_hal.spi->CR1 |= SPI_CR1_SPE;
		
		_tx_dma.TransferM2P(data_out, (void *)&_hal.spi->DR, num_data);	

		_rx_dma.SetCallback(_callback,_callback_args);
		_rx_dma.TransferP2M((void *)&_hal.spi->DR, data_in, num_data);	

		_hal.spi->CR2 |= SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN;

		return 0;
	}

};







#endif








