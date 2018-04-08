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
			
			SPI_ROLE role:1;
			//master or slave selection

			uint16_t:4; //padding to get the bits in the right spot for the registers

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
	SPI_ROLE_MASTER, \
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

	inline uint32_t PreTransmission(void)
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

	virtual inline uint32_t GetObjectData(void) { return 0; }
	virtual inline void SetObjectData(uint32_t data) { data = data; } 
	//get and set object data

public:
	inline SpiHal * GetHal(void) { return _hal; }	
	//get hal associated with driver

	inline void CallInterrupt(void) 
	{ if(_interrupt != 0){ _interrupt(_interrupt_args);} }
	inline void (*GetInterrupt(void))(void *args) { return _interrupt; }
	inline void * GetInterruptArgs(void) { return _interrupt_args; }
	inline SpiObject * SetInterrupt(
		void (*interrupt)(void *args), 
		void *interrupt_args)
	{ _interrupt = interrupt; _interrupt_args = interrupt_args; return this;}
	inline SpiObject * ResetInterrupt(void)
	{	return SetInterrupt(0,0); }
	//call get and set interrupt

	inline void CallCallback(void) 
	{ if(_callback != 0){ _callback(_callback_args);} }
	inline void (*GetCallback(void))(void *args) { return _callback; }
	inline void * GetCallbackArgs(void) { return _callback_args; }
	inline SpiObject * SetCallback(
		void (*callback)(void *args), 
		void *callback_args)
	{ _callback = callback; _callback_args = callback_args; return this;}
	inline SpiObject * ResetCallback(void)
	{	return SetCallback(0,0); }
	//call get and set Callback

	inline SpiObjectSettings GetSettings(void) {return _settings; }
	//Get settings

	inline SPI_CLOCK_PHASE GetClockPhase(void) {return _settings.clock_phase; }
	inline SPI_CLOCK_POLARITY GetClockPolarity(void) 
	{return _settings.clock_polarity; }
	inline SPI_BIT_ORDER GetBitOrder(void) {return _settings.bit_order; }
	inline SPI_DATA_LENGTH GetDataLength(void) {return _settings.data_length; }
	//get set individual settings functions

	inline uint32_t GetClock(void) { return _clock_frequency; }
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
	//Get config clock frequency

	virtual inline uint32_t Status(void) {return 1;}
	//Spi status
	
	virtual inline SpiObject * Stop(void) {return this;}
	//Spi Stop

	virtual inline uint32_t Transmit(void *data_out, uint16_t num_data)
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
	virtual inline uint32_t Transfer(
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
	virtual inline uint32_t Receive(void *data_in, uint16_t num_data)
	{
		data_in = data_in;
		num_data = num_data;

		return 1;
	}
	//Transmit Receive Transfer

	SpiObject(SpiHal *hal)
	{
		_hal = hal;
		
		if(hal->num_owners++ == 0)
		{
			NvicEnableHalInterrupt(&hal->nvic);
			RccEnableClock(&hal->rcc);
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

class SpiPolled : public SpiObject
{
	inline uint32_t GetObjectData(void) { return 0; }
	inline void SetObjectData(uint32_t data) { data = data; } 
	//get and set object data

public:
	inline uint32_t Status(void) 
	{ if((GetHal()->spi->SR & SPI_SR_BSY) != 0) { return 1; } else { return 0; } }

	inline SpiObject * Stop(void) 
	{ GetHal()->spi->CR1 &= ~SPI_CR1_SPE; return this; }

	inline uint32_t Transmit(void *data_out, uint16_t num_data)
	{
		SpiObject::Transmit(data_out, num_data);

		GetHal()->spi->CR1 |= SPI_CR1_SPE;

		SPI_BIT_ORDER dff = GetBitOrder();
		//get 8 bit or 16 bit data

		do
		{
			if(dff == 0)
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

	inline uint32_t Transfer(void *data_out, void *data_in, uint16_t num_data)
	{
		SpiObject::Transfer(data_out, data_in, num_data);

		GetHal()->spi->CR1 |= SPI_CR1_SPE;

		SPI_BIT_ORDER dff = GetBitOrder();
		//get 8 bit or 16 bit data

		do
		{
			if(dff == 0)
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
	
			if(dff == 0)
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
	inline uint32_t Receive(void *data_in, uint16_t num_data)
	{
		data_in = data_in;
		num_data = num_data;

		return 1;
	}

	SpiPolled(SpiHal *hal)
	: SpiObject(hal)
	{}
	SpiPolled(SpiHal *hal, SpiObjectSettings settings)
	: SpiObject(hal, settings)
	{}

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








