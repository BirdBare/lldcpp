//
//
//
//
//



#ifndef GPIO_H
#define GPIO_H

#include "board.h"
#include "rcc_lld.h"




	enum GPIO_PIN
	{
		GPIO_PIN_0 = 0x0001,
		GPIO_PIN_1 = 0x0002,
		GPIO_PIN_2 = 0x0004,
		GPIO_PIN_3 = 0x0008,
		GPIO_PIN_4 = 0x0010,
		GPIO_PIN_5 = 0x0020,
		GPIO_PIN_6 = 0x0040,
		GPIO_PIN_7 = 0x0080,
		GPIO_PIN_8 = 0x0100,
		GPIO_PIN_9 = 0x0200,
		GPIO_PIN_10 = 0x0400,
		GPIO_PIN_11 = 0x0800,
		GPIO_PIN_12 = 0x1000,
		GPIO_PIN_13 = 0x2000,
		GPIO_PIN_14 = 0x4000,
		GPIO_PIN_15 = 0x8000,
		GPIO_PIN_ALL = 0xFFFF
	};

	enum GPIO_MODE
	{
		GPIO_MODE_INPUT = 0b00,
		GPIO_MODE_OUTPUT = 0b01,
		GPIO_MODE_ALT = 0b10,
		GPIO_MODE_ANALOG = 0b11
	};

	enum GPIO_TYPE
	{
		GPIO_TYPE_PUSHPULL = 0b0,
		GPIO_TYPE_OPENDRAIN = 0b1
	};

	enum GPIO_PUPD
	{
		GPIO_PUPD_OFF = 0b00,
		GPIO_PUPD_PU = 0b01,
		GPIO_PUPD_PD = 0b10
	};

	enum GPIO_ALT
	{
		GPIO_ALT_0 = 0x00,
		GPIO_ALT_1 = 0x01,
		GPIO_ALT_2 = 0x02,
		GPIO_ALT_3 = 0x03,
		GPIO_ALT_4 = 0x04,
		GPIO_ALT_5 = 0x05,
		GPIO_ALT_6 = 0x06,
		GPIO_ALT_7 = 0x07,
		GPIO_ALT_8 = 0x08,
		GPIO_ALT_9 = 0x09,
		GPIO_ALT_10 = 0x0A,
		GPIO_ALT_11 = 0x0B,
		GPIO_ALT_12 = 0x0C,
		GPIO_ALT_13 = 0x0D,
		GPIO_ALT_14 = 0x0E,
		GPIO_ALT_15 = 0x0F
	};


//******************************************************************************
//
//
//
//******************************************************************************
struct GpioHal
{
	const struct RccHal rcc; //clock object for clock register and bit location

	volatile GPIO_TypeDef * const gpio; //gpio pointer to gpio register base

	uint32_t used_pins = 0; //used pins on this gpio port. acts as configured pins
};

extern struct GpioHal
	GPIOA_HAL,
	GPIOB_HAL,
	GPIOC_HAL,
	GPIOD_HAL,
	GPIOE_HAL,
	GPIOF_HAL,
	GPIOG_HAL,
	GPIOH_HAL,
	GPIOI_HAL,
	GPIOJ_HAL,
	GPIOK_HAL;



//******************************************************************************
//
//
//
//******************************************************************************
struct GpioSettings
{
	GpioSettings& operator=(const GpioSettings &copy)
	{ *(uint32_t *)this = *(uint32_t *)&copy; return *this; }

	inline GPIO_TYPE Type(void) { return type; }
	inline GpioSettings& Type(GPIO_TYPE _type) { type = _type; return *this;}

	inline GPIO_PUPD PuPd(void) { return pupd; }
	inline GpioSettings& PuPd(GPIO_PUPD _pupd) { pupd = _pupd; return *this; }

	inline GPIO_ALT Alt(void) {return alt;}
	inline GpioSettings Alt(GPIO_ALT _alt) {alt = _alt; return *this;}

	union
	{
		struct
		{
			GPIO_TYPE type:8;
			GPIO_PUPD pupd:8;
			GPIO_ALT alt:8;
			uint32_t:8;
		};
		
		uint32_t settings = 0;
	};
};



//******************************************************************************
//
//
//
//******************************************************************************
class GpioObject
{
protected:

	GpioHal &_hal;
	//hal object for actually configuring pins

	struct GpioSettings _settings;
	//gpio settings for each type
	
	uint32_t _pins = 0; 
	//pins associated with this object

	inline GpioObject& ResetConfig(void) { _hal.used_pins &= ~_pins; return *this;}
	//config reset functions

public:	
	inline uint32_t Pins(void) { return _pins; }
	//Add Get Pin functions

	GpioObject(GpioHal &hal, uint32_t pins, const GpioSettings &settings) 
	: _hal(hal)
	{ 
		_pins = pins;
		_settings = settings;

		if(_hal.used_pins == 0)
		{
			RccEnableClock(&_hal.rcc);
			_hal.gpio->OSPEEDR = 0xffffffff;
		}
	}
	GpioObject(GpioHal &hal, GPIO_PIN pin, const GpioSettings &settings)
	: GpioObject(hal, (uint32_t)pin, settings)
	{}
	//constructor for gpio object

	~GpioObject()
	{
		ResetConfig();
		if(_hal.used_pins == 0)
		{
			RccDisableClock(&_hal.rcc);
		}
	}
	//destructor for gpio object
};




//******************************************************************************
//
//
//
//******************************************************************************
class GpioOutput : public GpioObject
{
	uint32_t Config(void);
	//direction dependent configuration function

public:
	GpioOutput(GpioHal &hal, uint32_t pins, const GpioSettings &settings = {}) 
	: GpioObject(hal, pins, settings) { Config(); }
	GpioOutput(GpioHal &hal, GPIO_PIN pin, const GpioSettings &settings = {}) 
	: GpioOutput(hal, (uint32_t) pin, settings) 
	{}
	//constructor for gpioOutput

	inline GpioOutput& Set(uint32_t pins = GPIO_PIN_ALL) 
		{ _hal.gpio->BSRR = pins & _pins; return *this; }	
	inline GpioOutput& Toggle(uint32_t pins = GPIO_PIN_ALL) 
		{ _hal.gpio->ODR ^= pins & _pins; return *this; }	
	inline GpioOutput& Reset(uint32_t pins = GPIO_PIN_ALL) 
		{ _hal.gpio->BSRR = (pins & _pins) << 16; return *this;}	
	//set toggle and reset pin or pins

	inline uint32_t Get(uint32_t pins = GPIO_PIN_ALL)
		{ return _hal.gpio->ODR & pins & _pins; }
	//get value of pin. set or reset
};




//******************************************************************************
//
//
//
//******************************************************************************
class GpioInput : public GpioObject
{
	uint32_t Config(void);
	//direction dependent configuration function
public:
	GpioInput(GpioHal &hal, uint32_t pins, const GpioSettings &settings = {})
		: GpioObject(hal,pins, settings) 
		{ Config(); }	
	GpioInput(GpioHal &hal, GPIO_PIN pin, const GpioSettings &settings = {})
		: GpioInput(hal,(uint32_t)pin,settings)
		{}

	inline uint32_t Get(uint32_t pins = GPIO_PIN_ALL)
		{ return _hal.gpio->IDR & pins & _pins; }
	//get value of pin. set or reset
};


//******************************************************************************
//
//
//
//******************************************************************************
class GpioAlt : public GpioObject
{
	uint32_t Config(void);
	//direction dependent configuration function
public:
	GpioAlt(GpioHal &hal, uint32_t pins, const GpioSettings &settings = {}) 
	: GpioObject(hal,pins, settings) 
	{ Config(); }
	GpioAlt(GpioHal &hal, GPIO_PIN pin, const GpioSettings &settings = {}) 
	: GpioAlt(hal,(uint32_t)pin,settings)
	{}
	//constructor for gpioAlternate
};


//******************************************************************************
//
//
//
//******************************************************************************
class GpioAnalog : public GpioObject
{
	uint32_t Config(void);

public:
	GpioAnalog(GpioHal &hal, uint32_t pins) 
	: GpioObject(hal, pins, {}) 
	{ Config(); }	
	GpioAnalog(GpioHal &hal, GPIO_PIN pin) 
	: GpioAnalog(hal, (uint32_t)pin) 
	{}	
};


#endif
