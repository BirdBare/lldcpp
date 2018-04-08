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
class GpioObject
{
protected:

	GpioHal *_hal;
	//hal object for actually configuring pins
	
	uint32_t _pins = 0; 
	//pins associated with this object

	inline uint32_t ResetConfig(void) { _hal->used_pins &= ~_pins; return 0; }	
	//config reset functions

public:	
	inline GpioHal * GetHal(void) { return _hal; }
	//return hal object

	inline uint32_t GetPins(void) { return _pins; }
	//Add Get Pin functions


	GpioObject(GpioHal *hal, uint32_t pins) 
	{ 
		_pins = pins;
		_hal = hal; 
		if(_hal->used_pins == 0)
		{
			RccEnableClock(&_hal->rcc);
			_hal->gpio->OSPEEDR = 0xffffffff;
		}
	}
	GpioObject(GpioHal *hal, GPIO_PIN pin)
	: GpioObject(hal, (uint32_t)pin)
	{}
	//constructor for gpio object

	~GpioObject()
	{
		ResetConfig();
		if(_hal->used_pins == 0)
		{
			RccDisableClock(&_hal->rcc);
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
	GPIO_TYPE _type;
	//output type

	GPIO_PUPD _pupd;
	//output pullup or down resistors

	uint32_t Config(void);
	//direction dependent configuration function

public:
	GpioOutput(GpioHal *hal, uint32_t pins, 
		GPIO_TYPE type, GPIO_PUPD pupd) 
	: GpioObject(hal, pins) { _type = type; _pupd = pupd; Config(); }
	GpioOutput(GpioHal *hal, GPIO_PIN pin, 
		GPIO_TYPE type, GPIO_PUPD pupd) 
	: GpioOutput(hal, (uint32_t) pin, type, pupd) 
	{}
	//constructor for gpioOutput

	inline class GpioOutput * Set(uint32_t pins = GPIO_PIN_ALL) 
		{ _hal->gpio->BSRR = pins & _pins; return this; }	
	inline class GpioOutput * Toggle(uint32_t pins = GPIO_PIN_ALL) 
		{ _hal->gpio->ODR ^= pins & _pins; return this; }	
	inline class GpioOutput * Reset(uint32_t pins = GPIO_PIN_ALL) 
		{ _hal->gpio->BSRR = (pins & _pins) << 16; return this;}	
	//set toggle and reset pin or pins

	inline uint32_t Get(uint32_t pins = GPIO_PIN_ALL)
		{ return _hal->gpio->ODR & pins & _pins; }
	//get value of pin. set or reset
};




//******************************************************************************
//
//
//
//******************************************************************************
class GpioInput : public GpioObject
{
	GPIO_PUPD _pupd;
	//output pullup or down resistors

	uint32_t Config(void);
	//direction dependent configuration function
public:
	GpioInput(GpioHal *hal, uint32_t pins,
		GPIO_PUPD pupd) 
		: GpioObject(hal,pins) 
		{ _pupd = pupd; Config(); }	
	GpioInput(GpioHal *hal, GPIO_PIN pin,
		GPIO_PUPD pupd) 
		: GpioInput(hal,(uint32_t)pin,pupd)
		{}

	inline uint32_t Get(uint32_t pins = GPIO_PIN_ALL)
		{ return _hal->gpio->IDR & pins & _pins; }
	//get value of pin. set or reset
};


//******************************************************************************
//
//
//
//******************************************************************************
class GpioAlt : public GpioObject
{
	GPIO_TYPE _type;
	//output type

	GPIO_PUPD _pupd;
	//output pullup or down resistors

	GPIO_ALT _alt;
	//gpio alternate function

	uint32_t Config(void);
	//direction dependent configuration function
public:
	GpioAlt(GpioHal *hal, uint32_t pins, 
		GPIO_ALT alt, GPIO_TYPE type, GPIO_PUPD pupd) 
	: GpioObject(hal,pins) 
	{ _alt = alt; _type = type; _pupd = pupd; Config(); }
	GpioAlt(GpioHal *hal, GPIO_PIN pin, 
		GPIO_ALT alt, GPIO_TYPE type, GPIO_PUPD pupd) 
	: GpioAlt(hal,(uint32_t)pin,alt,type,pupd)
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
	GpioAnalog(GpioHal *hal, uint32_t pins) 
	: GpioObject(hal, pins) 
	{ Config(); }	
	GpioAnalog(GpioHal *hal, GPIO_PIN pin) 
	: GpioAnalog(hal, (uint32_t)pin) 
	{}	
};


#endif
