//
//
//
//
//



#ifndef SPI_OBJECT_HPP
#define SPI_OBJECT_HPP

#include "spi_base.hpp"



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


struct SpiObjectSettings
{
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

	uint32_t ClockFrequency(void) ;
	SpiObjectSettings& ClockFrequency(uint32_t freq);

	SPI_CLOCK_PHASE ClockPhase(void) ;
	SpiObjectSettings& ClockPhase(SPI_CLOCK_PHASE phase) ;
	//get and set clock phase

	bool Master(void) ;
	SpiObjectSettings& Master(bool state);
	//master initialization

	SPI_CLOCK_POLARITY ClockPolarity(void) ;
	SpiObjectSettings& ClockPolarity(SPI_CLOCK_POLARITY polarity); 
	//get and set polarity
	
	SPI_BIT_ORDER BitOrder(void) ;
	SpiObjectSettings& BitOrder(SPI_BIT_ORDER order) ;
	//get and set bit order
	
	uint32_t DataSize(void) ;
	SpiObjectSettings& DataSize(uint32_t size) ;
	//get and set data size. takes and return number of bytes

	uint32_t CrcPolynomial(void) ;
	SpiObjectSettings& CrcPolynomial(uint32_t polynomial);

};

//******************************************************************************
//
//
//
//******************************************************************************
	inline uint32_t SpiObjectSettings::ClockFrequency(void) 
	{
		return clock_frequency;
	}
	inline SpiObjectSettings& SpiObjectSettings::ClockFrequency(uint32_t freq)
	{	
		clock_frequency = freq; 
		return *this;
	}


//******************************************************************************
//
//
//
//******************************************************************************
	inline SPI_CLOCK_PHASE SpiObjectSettings::ClockPhase(void) 
	{
		return clock_phase; 
	}
	inline SpiObjectSettings& SpiObjectSettings::ClockPhase(SPI_CLOCK_PHASE phase) 
	{
		clock_phase = phase; 
		return *this;
	}	
	//get and set clock phase


//******************************************************************************
//
//
//
//******************************************************************************
	inline bool SpiObjectSettings::Master(void) 
	{ 
		return master; 
	}
	inline SpiObjectSettings& SpiObjectSettings::Master(bool state)
	{ 
		master = state; 
		return *this;
	}
	//master initialization


//******************************************************************************
//
//
//
//******************************************************************************
	inline SPI_CLOCK_POLARITY SpiObjectSettings::ClockPolarity(void) 
	{
		return clock_polarity; 
	}
	inline SpiObjectSettings& SpiObjectSettings::ClockPolarity(
		SPI_CLOCK_POLARITY polarity) 
	{
		clock_polarity = polarity; 
		return *this;
	}
	//get and set polarity
	

//******************************************************************************
//
//
//
//******************************************************************************
	inline SPI_BIT_ORDER SpiObjectSettings::BitOrder(void) 
	{
		return bit_order;
	}
	inline SpiObjectSettings& SpiObjectSettings::BitOrder(SPI_BIT_ORDER order) 
	{
		bit_order = order; 
		return *this;
	}
	//get and set bit order
	

//******************************************************************************
//
//
//
//******************************************************************************
	inline uint32_t SpiObjectSettings::DataSize(void) 
	{
		return 8 << data_size; 
	}
	inline SpiObjectSettings& SpiObjectSettings::DataSize(uint32_t size) 
	{
		data_size = size >> 4; 
		return *this;
	}
	//get and set data size. takes and return number of bytes


//******************************************************************************
//
//
//
//******************************************************************************
	inline uint32_t SpiObjectSettings::CrcPolynomial(void) 
	{
		return crc_polynomial;
	}
	inline SpiObjectSettings& SpiObjectSettings::CrcPolynomial(uint32_t polynomial)
	{ 
		crc_polynomial = polynomial; 
		return *this;
	}




//******************************************************************************
//
//
//
//******************************************************************************
class SpiObject : public SpiBase
{
protected:
	SpiObjectSettings _settings;
	//settings for object

	void PreTransmission(void);

	SpiObject(SpiHal &hal)
	: SpiBase(hal), _settings()
	{}
	
public:

	SpiObjectSettings& Settings(void) 
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




#endif








