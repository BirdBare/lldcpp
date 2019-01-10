//
//
//
//
//



#ifndef GPIO_H
#define GPIO_H

#include "board.h"
#include "rcc_lld.h"


//******************************************************************************
//
//
//
//******************************************************************************
struct GpioPort
{
	struct RccLld rcc; //clock object for clock register and bit location

	volatile GPIO_TypeDef * const gpio; //gpio pointer to gpio register base
};



extern struct GpioPort
	GPIOA_PORT,
	GPIOB_PORT,
	GPIOC_PORT,
	GPIOD_PORT,
	GPIOE_PORT,
	GPIOF_PORT,
	GPIOG_PORT,
	GPIOH_PORT,
	GPIOI_PORT,
	GPIOJ_PORT,
	GPIOK_PORT;



//******************************************************************************
//
//
//
//******************************************************************************
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

	enum GPIO_INT
	{
		GPIO_INT_RISE,
		GPIO_INT_FALL,
		GPIO_INT_RISEFALL
	};

//******************************************************************************
//
//
//
//******************************************************************************
struct GpioSettings
{
	uint32_t _pins;

	enum GPIO_MODE _mode;

	enum GPIO_TYPE _type;

	enum GPIO_PUPD _pupd;

	enum GPIO_ALT _alt;
};

//******************************************************************************
//
//
//
//******************************************************************************
static inline void GpioSetPins(struct GpioSettings *settings, uint32_t pins)
{
	settings->_pins = pins;
}
static inline uint32_t GpioGetPins(struct GpioSettings *settings)
{
	return settings->_pins;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void GpioSetMode(struct GpioSettings *settings, enum GPIO_MODE mode)
{
	settings->_mode = mode;
}
static inline enum GPIO_MODE GpioGetMode(const struct GpioSettings *settings)
{
	return settings->_mode;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void GpioSetType(struct GpioSettings *settings, enum GPIO_TYPE type)
{
	settings->_type = type;
}
static inline enum GPIO_TYPE GpioGetType(const struct GpioSettings *settings)
{
	return settings->_type;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void GpioSetPuPd(struct GpioSettings *settings, enum GPIO_PUPD pupd)
{
	settings->_pupd = pupd;
}
static inline enum GPIO_PUPD GpioGetPuPd(const struct GpioSettings *settings)
{
	return settings->_pupd;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void GpioSetAlt(struct GpioSettings *settings, enum GPIO_ALT alt)
{
	settings->_alt = alt;
}
static inline enum GPIO_ALT GpioGetAlt(const struct GpioSettings *settings)
{
	return settings->_alt;
}


//******************************************************************************
//
//
//
//******************************************************************************
typedef struct GpioPort GpioPort_t;
typedef struct GpioSettings GpioSettings_t;


//******************************************************************************
//
//
//
//******************************************************************************
static inline bool GpioInit(struct GpioPort *port)
{
	RccLldInit(&port->rcc);
	port->gpio->OSPEEDR = 0xffffffff;

	return true;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline bool GpioDeinit(struct GpioPort *port)
{
	RccLldDeinit(&port->rcc);

	return true;
}


//******************************************************************************
//
//
//
//******************************************************************************
void GpioConfig(volatile GPIO_TypeDef * const gpio_port,
	const struct GpioSettings *settings);

//******************************************************************************
//
//
//
//******************************************************************************
static inline bool GpioDeconfig(struct GpioPort *port,
	struct GpioSettings *settings)
{
	GpioSetMode(settings,GPIO_MODE_INPUT);
	GpioSetType(settings,GPIO_TYPE_PUSHPULL);
	GpioSetPuPd(settings,GPIO_PUPD_OFF);
	GpioSetAlt(settings,GPIO_ALT_0);
	GpioConfig(port->gpio,settings);
	return true;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline bool GpioConfigOutput(struct GpioPort *port, 
	struct GpioSettings *settings)
{
	GpioSetMode(settings,GPIO_MODE_OUTPUT);
	GpioConfig(port->gpio,settings);
	return true;
}

static inline bool GpioConfigInput(struct GpioPort *port, 
	struct GpioSettings *settings)
{
	GpioSetMode(settings,GPIO_MODE_INPUT);
	GpioConfig(port->gpio,settings);
	return true;
}

static inline bool GpioConfigAlternate(struct GpioPort *port, 
	struct GpioSettings *settings)
{
	GpioSetMode(settings,GPIO_MODE_ALT);
	GpioConfig(port->gpio,settings);
	return true;
}

static inline bool GpioConfigAnalog(struct GpioPort *port, 
	struct GpioSettings *settings)
{
	GpioSetMode(settings,GPIO_MODE_ANALOG);
	GpioConfig(port->gpio,settings);
	return true;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void GpioSetPin(struct GpioPort *port, uint32_t pins)
{
	port->gpio->BSRR = pins;
}

static inline void GpioResetPin(struct GpioPort *port, uint32_t pins)
{
	port->gpio->BSRR = pins << 16;
}

static inline void GpioTogglePin(struct GpioPort *port, uint32_t pins)
{
	port->gpio->ODR ^= pins;
}

static inline uint32_t GpioGetOutput(struct GpioPort *port, uint32_t pins)
{
	return port->gpio->ODR & pins;
}


static inline uint32_t GpioGetInput(struct GpioPort *port, uint32_t pins)
{
	return port->gpio->IDR & pins;
}






#endif
