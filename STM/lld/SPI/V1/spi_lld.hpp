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

enum SPI_BIT_ORDER
{
	SPI_BIT_ORDER_MSB = 0,
	SPI_BIT_ORDER_LSB = 1
};

enum SPI_DATA_LENGTH
{
	SPI_DATA_LENGTH_8 = 0,
	SPI_DATA_LENGTH_16 = 1
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

	uint16_t tx_num_data;
	uint16_t rx_num_data;
	//num data left. Used to stop the spi
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
struct SpiObjectSettings
{
	SpiObjectSettings& operator=(const SpiObjectSettings &copy)
	{	*(uint16_t *)this = *(uint16_t *)&copy; return *this; }

	union
	{
		struct
		{
			//LSB
			SPI_CLOCK_PHASE clock_phase:1; 
			//When the data is sampled. first or second edge

			SPI_CLOCK_POLARITY clock_polarity:1; 
			//the voltage of the clock when starting

			uint16_t:5; //padding to get the bits in the right spot for the registers

			SPI_BIT_ORDER bit_order:1; 
			//msb first or lsb first

			uint16_t:3; //padding to get the bits in the right spot for the registers

			SPI_DATA_LENGTH data_length:1; 
			//length of the data in bits

			uint16_t:4; //padding to get the bits in the right spot for the registers
			//MSB
		};

		uint16_t cr1;
		//register like access to settings
	};

	uint16_t crc_polynomial = 0; 
	//crc polynomial register

};



#define SPI_SETTINGS_DEFAULT \
{	SPI_CLOCK_PHASE_FIRST, \
	SPI_CLOCK_POLARITY_LOW, \
	SPI_BIT_ORDER_MSB, \
	SPI_DATA_LENGTH_8, \
	0};



//******************************************************************************
//
//
//
//******************************************************************************
class SpiObject
{
	SpiHal *_hal;

	uint32_t _clock_frequency = 0; 
	//spi clock frequency. config calculates be to actual

	void *_interrupt_args = 0; //arguments for interrupt
	void (*_interrupt)(void *args) = 0; //respective spi is argument
																 //replaces default interrupt
																 //if not used then must be 0

	void *_callback_args = 0; //arguments callback function
	void (*_callback)(void *args) = 0; //callback function for end of transfer

	SpiObjectSettings _settings = SPI_SETTINGS_DEFAULT;
		//settings for object

public:
	inline SpiHal * GetHal(void) { return _hal; }	
	//get hal associated with driver

	inline uint32_t GetClockFrequency(void) { return _clock_frequency; }
	//get clock frequency
	
	inline void (*GetInterrupt(void))(void *args) { return _interrupt; }
	inline void * GetInterruptArgs(void) { return _interrupt_args; }
	inline SpiObject * SetInterrupt(
		void (*interrupt)(void *args), 
		void *interrupt_args)
	{ _interrupt = interrupt; _interrupt_args = interrupt_args; return this;}
	inline SpiObject * ResetInterrupt(void)
	{	return SetInterrupt(0,0); }
	//get and set interrupt

	inline void (*GetCallback(void))(void *args) { return _callback; }
	inline void * GetCallbackArgs(void) { return _callback_args; }
	inline SpiObject * SetCallback(
		void (*callback)(void *args), 
		void *callback_args)
	{ _callback = callback; _callback_args = callback_args; return this;}
	inline SpiObject * ResetCallback(void)
	{	return SetCallback(0,0); }
	//get and set Callback

	inline SpiObjectSettings GetSettings(void) {return _settings; }
	//Get settings

	inline SPI_CLOCK_PHASE GetClockPhase(void) {return _settings.clock_phase; }
	inline SPI_CLOCK_POLARITY GetClockPolarity(void) 
	{return _settings.clock_polarity; }
	inline SPI_BIT_ORDER GetBitOrder(void) {return _settings.bit_order; }
	inline SPI_DATA_LENGTH GetDataLength(void) {return _settings.data_length; }
	//get set individual settings functions

	inline uint32_t Status(void)
	{ 
		return (_hal->spi->SR & SPI_SR_BSY) != 0 ||
			_hal->tx_num_data != 0 || _hal->rx_num_data != 0;
	}
	//Spi status
	
	inline SpiObject * Stop(void)
	{ 
		_hal->tx_num_data = 0; 
		_hal->rx_num_data = 0; 
		return this; 
	}
	//Spi Stop
	
	inline void ConfigClock(uint32_t clock_frequency = 0)
	{
		uint32_t _clock_frequency = RccGetPeripheralSpeed(&_hal->rcc);
		//get bus speed for calculation

		uint32_t br = 0 - 1;
		//counter starts at zero so we need to overflow to zero

		do
		{
			_clock_frequency >>= 1;
			br++;
		} while(clock_frequency < _clock_frequency && br < 7);
		//gets correct register br value and calculates actual clock speed
		
		*(uint32_t *)&_settings |= br << 3;
	}
	//config clock frequency

	virtual inline uint32_t Transmit()
	{
		_hal->spi->CR1 = _settings.cr1;
		//"reset" spi settings for new transfer

		_hal->spi->CR2 = SPI_CR2_SSOE | SPI_CR2_ERRIE;
		//"reset" cr2

		if(_interrupt != 0)
		{
			_hal->spi->CR2 |= SPI_CR2_TXEIE;
		}
		//if interrupt is set then enable interrupt

		_hal->spi->DR; //dummy read

		_hal->spi->CRCPR = _settings.crc_polynomial;

		return 0;
	}
	//transmit, transfer, receive

	SpiObject(SpiHal *hal)
	{
		_hal = hal;
		
		if(hal->num_owners++ == 0)
		{
			NvicEnableHalInterrupt(&hal->nvic);
			RccEnableClock(&hal->rcc);

			hal->tx_num_data = 0;
			hal->rx_num_data = 0;
		}
		//init the object if it has never been connected
	
			}
	SpiObject(
	SpiHal *hal, 
	SpiObjectSettings settings)
	: SpiObject(hal)
	{	_settings = settings; }
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











/*

//######### WRITING OWN INTERRUPT FUNCTIONS

//High means transmit is ready to accept data
static inline uint32_t LldSpiTransmitReady(struct SpiHal *spi_object)
{
	return (spi_object->spi->SR & SPI_SR_TXE) && 
		(spi_object->spi->CR2 & SPI_CR2_TXEIE);
}

//high means receive is ready to give data
static inline uint32_t LldSpiReceiveReady(struct SpiHal *spi_object)
{
	return spi_object->spi->SR & SPI_SR_RXNE &&
		(spi_object->spi->CR2 & SPI_CR2_RXNEIE);
}

static inline void LldSpiTxDisableInterrupt(struct SpiHal *spi_object)
{
	spi_object->spi->CR2 &= ~SPI_CR2_TXEIE;
}

static inline void LldSpiRxDisableInterrupt(struct SpiHal *spi_object)
{
	spi_object->spi->CR2 &= ~SPI_CR2_RXNEIE;
}

void LldSpiPutDataHal(struct SpiHal *spi_object, uint32_t data);

uint32_t LldSpiGetDataHal(struct SpiHal *spi_object);

void LldSpiPutDataDevice(struct SpiHal *spi_object, uint32_t data);

uint32_t LldSpiGetDataDevice(struct SpiHal *spi_object);

uint32_t LldSpiTxDecrementNumData(struct SpiHal *spi_object);

uint32_t LldSpiRxDecrementNumData(struct SpiHal *spi_object);

static inline void LldSpiCallCallback(struct SpiHal *spi_object)
{
	if(spi_object->spi_config->callback != 0)
	{
		spi_object->spi_config->callback(spi_object->spi_config->callback_args);
	}
}
//#########################################

*/


#endif








