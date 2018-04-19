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
#include "gpio_lld.hpp"
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

enum SPI_ROLE
{
	SPI_ROLE_SLAVE = 0,
	SPI_ROLE_MASTER = 1
};

enum SPI_BIT_ORDER
{
	SPI_BIT_ORDER_MSB = 0,
	SPI_BIT_ORDER_LSB = 1
};

enum SPI_DATA_SIZE
{
	SPI_DATA_SIZE_8 = 0,
	SPI_DATA_SIZE_16 = 1
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

	uint8_t tx_dma_channel; 
	uint8_t rx_dma_channel;

	struct DmaHal *tx_dma_object;
	struct DmaHal *rx_dma_object;

	volatile SPI_TypeDef * const spi;

	class SpiObject *owner = 0;
	//object that owns this spi

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
			
			SPI_ROLE role:1;
			//master or slave selection

			uint16_t:4; //padding to get the bits in the right spot for the registers

			SPI_BIT_ORDER bit_order:1; 
			//msb first or lsb first

			uint16_t:3; //padding to get the bits in the right spot for the registers

			SPI_DATA_SIZE data_size:1; 
			//size of the data in bits

			uint16_t:4; //padding to get the bits in the right spot for the registers
			//MSB
		};

		uint16_t cr1;
		//register like access to settings
	};

	uint16_t crc_polynomial = 0; 
	//crc polynomial register

};



#define SPI_DEFAULT_SETTINGS \
{	0, \
	SPI_CLOCK_PHASE_FIRST, \
	SPI_CLOCK_POLARITY_LOW, \
	SPI_ROLE_MASTER, \
	SPI_BIT_ORDER_MSB, \
	SPI_DATA_SIZE_8, \
	0}



//******************************************************************************
//
//
//
//******************************************************************************
class SpiObject
{
protected:
	SpiHal *_hal;
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
	inline SpiHal * GetHal(void) { return _hal; }	
	//get hal associated with driver

/*
	virtual inline uint32_t GetObjectData(void) = 0;
	virtual inline void SetObjectData(uint32_t data) = 0; 
	//get and set object data

	inline void (*GetInterrupt(void))(void *args) { return _interrupt; }
	inline void * GetInterruptArgs(void) { return _interrupt_args; }
	inline void SetInterrupt(
		void (*interrupt)(void *args), 
		void *interrupt_args)
	{ _interrupt = interrupt; _interrupt_args = interrupt_args; }
	inline void ResetInterrupt(void)
	{	SetInterrupt(0,0); }
	//call get and set interrupt

	virtual void (*GetCallback(void))(void *args) = 0;
	virtual void * GetCallbackArgs(void) = 0; 
	virtual void SetCallback(
		void (*callback)(void *args), 
		void *callback_args) = 0;
	virtual void ResetCallback(void) = 0;
	//call get and set Callback
*/

	inline SpiSettings GetSettings(void) {return _settings; }
	//Get settings

	inline SPI_CLOCK_PHASE GetClockPhase(void) {return _settings.clock_phase; }
	inline SPI_CLOCK_POLARITY GetClockPolarity(void) 
	{return _settings.clock_polarity; }
	inline SPI_BIT_ORDER GetBitOrder(void) {return _settings.bit_order; }
	inline SPI_DATA_SIZE GetDataSize(void) {return _settings.data_size; }
	//get set individual settings functions

	inline uint32_t Receive(void *data_in, uint16_t num_data)
	{
		data_in = data_in;
		num_data = num_data;

		return 1;
	}
	//Transmit Receive Transfer

	SpiObject(SpiHal *hal, SpiSettings settings)
	{
		uint32_t clock_frequency = settings.clock_frequency;
		//user set clock frequency

		settings.clock_frequency = RccGetPeripheralSpeed(&hal->rcc);
		//get bus speed for calculation

		uint32_t br = 0 - 1;
		//counter starts at zero so we need to overflow to zero

		do
		{
			settings.clock_frequency >>= 1;
			br++;
		} while(clock_frequency < settings.clock_frequency && br < 7);
		//gets correct register br value and calculates actual clock speed

		settings.cr1 |= br << 3;
		//config clock frequency

		_settings = settings;
		_hal = hal;
		
		if(hal->num_owners++ == 0)
		{
			NvicEnableHalInterrupt(&hal->nvic);
			RccEnableClock(&hal->rcc);
		}
		//init the object if it has never been connected
	}
	//constructor for spi object
	
	~SpiObject()
	{
		if(--_hal->num_owners == 0)
		{
			RccResetPeripheral(&_hal->rcc);
			NvicDisableHalInterrupt(&_hal->nvic);
			RccDisableClock(&_hal->rcc);
		}
		//simple deinit object
	}
	//destructor for spi object

};

class SpiPolled : public SpiObject
{
public:
	inline uint32_t Status(void) 
	{ if((GetHal()->spi->SR & SPI_SR_BSY) != 0) { return 1; } else { return 0; } }
	//spi status

	inline SpiObject * Stop(void) 
	{ GetHal()->spi->CR1 &= ~SPI_CR1_SPE; return this; }
	//stop transfer

	uint32_t Transmit(void *data_out, uint16_t num_data);
	uint32_t Transfer(void *data_out, void *data_in, uint16_t num_data);
	//transmit and transfer functions

	SpiPolled(SpiHal *hal, SpiSettings settings = SPI_DEFAULT_SETTINGS)
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
	
	void SpiPutDataHal(uint32_t data);
	uint32_t SpiGetDataHal(void );
	void SpiPutDataDevice(uint32_t data);
	uint32_t SpiGetDataDevice(void );
	uint32_t SpiTxDecrementNumData(void );
	uint32_t SpiRxDecrementNumData(void );
	inline uint32_t SpiTransmitReady(void)
	{
		return (GetHal()->spi->SR & SPI_SR_TXE) && 
			(GetHal()->spi->CR2 & SPI_CR2_TXEIE);
	}
	inline uint32_t SpiReceiveReady(void)
	{
		return GetHal()->spi->SR & SPI_SR_RXNE &&
			(GetHal()->spi->CR2 & SPI_CR2_RXNEIE);
	}
	inline void SpiTxDisableInterrupt(void)
	{
		GetHal()->spi->CR2 &= ~SPI_CR2_TXEIE;
	}
	inline void SpiRxDisableInterrupt(void)
	{
		GetHal()->spi->CR2 &= ~SPI_CR2_RXNEIE;
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
		GetHal()->owner = this;

		PreTransmission();

		GetHal()->spi->CR1 |= SPI_CR1_SPE | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;

		GetHal()->spi->CR2 |= SPI_CR2_TXEIE;

		return 0;
	}

	uint32_t Transfer(void *data_out,void *data_in, uint16_t num_data)	
	{
		_tx_data = data_out;
		_tx_num_data = num_data;
		_rx_data = data_in;
		_rx_num_data = num_data;
		GetHal()->owner = this;

		PreTransmission();

		GetHal()->spi->CR1 |= SPI_CR1_SPE;


		GetHal()->spi->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;

		return 0;
	}

	SpiInterrupt(SpiHal *hal, SpiSettings settings = SPI_DEFAULT_SETTINGS)
	: SpiObject(hal, settings)
	{}

};













class SpiDma : public SpiObject
{
	void (*_callback)(void *args) = 0;
	void *_callback_args = 0;

public:

	SpiDma(SpiHal *hal, SpiSettings settings = SPI_DEFAULT_SETTINGS)
	: SpiObject(hal,settings)
	{

	}

	

};







#endif








