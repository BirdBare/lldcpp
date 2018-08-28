//
//
//
//
//



#ifndef GPIO_H
#define GPIO_H

#include "board.h"
#include "rcc_lld.hpp"




//******************************************************************************
//
//
//
//******************************************************************************
struct GpioHal
{
	struct RccHal rcc; //clock object for clock register and bit location

	volatile GPIO_TypeDef * const gpio; //gpio pointer to gpio register base
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
struct GpioSettings
{
	GPIO_MODE _mode;

	uint32_t _pins;

	GPIO_TYPE _type; 
	
	GPIO_PUPD _pupd;

	GPIO_ALT _alt;
};


//******************************************************************************
//
//
//
//******************************************************************************
static inline void SetPins(struct GpioSettings *settings, uint32_t pins)
{
	settings->_pins = pins;
}
static inline uint32_t GetPins(struct GpioSettings *settings)
{
	return settings->_pins;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void SetMode(struct GpioSettings *settings, GPIO_MODE mode)
{
	settings->_mode = mode;
}
static inline GPIO_MODE GetMode(struct GpioSettings *settings)
{
	return settings->_mode;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void SetType(struct GpioSettings *settings, GPIO_TYPE type)
{
	settings->_type = type;
}
static inline GPIO_TYPE GetType(struct GpioSettings *settings)
{
	return settings->_type;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void SetPuPd(struct GpioSettings *settings, GPIO_PUPD pupd)
{
	settings->_pupd = pupd;
}
static inline GPIO_PUPD GetPuPd(struct GpioSettings *settings)
{
	return settings->_pupd;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void SetAlt(struct GpioSettings *settings, GPIO_ALT alt)
{
	settings->_alt = alt;
}
static inline GPIO_ALT GetAlt(struct GpioSettings *settings)
{
	return settings->_alt;
}


//******************************************************************************
//
//
//
//******************************************************************************
typedef struct GpioHal _GpioPort;
typedef struct GpioSettings _GpioSettings;


//******************************************************************************
//
//
//
//******************************************************************************
static inline bool LldGpioPortInit(struct GpioHal *hal)
{
	hal->rcc.Init();
	hal->gpio->OSPEEDR = 0xffffffff;

	return true;
}


//******************************************************************************
//
//
//
//******************************************************************************
void Config(volatile GPIO_TypeDef * const gpio_port,
	const struct GpioSettings *settings);

static inline bool LldGpioPortConfigOutput(struct GpioHal *hal, 
	struct GpioSettings *settings)
{
	SetMode(settings,GPIO_MODE_OUTPUT);
	Config(hal->gpio,settings);
	return true;
}

static inline bool LldGpioPortConfigInput(struct GpioHal *hal, 
	struct GpioSettings *settings)
{
	SetMode(settings,GPIO_MODE_INPUT);
	Config(hal->gpio,settings);
	return true;
}

static inline bool LldGpioPortConfigAlternate(struct GpioHal *hal, 
	struct GpioSettings *settings)
{
	SetMode(settings,GPIO_MODE_ALT);
	Config(hal->gpio,settings);
	return true;
}

static inline bool LldGpioPortConfigAnalog(struct GpioHal *hal, 
	struct GpioSettings *settings)
{
	SetMode(settings,GPIO_MODE_ANALOG);
	Config(hal->gpio,settings);
	return true;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline void LldGpioPortSetPin(struct GpioHal *hal, uint32_t pins)
{
	hal->gpio->BSRR = pins;
}

static inline void LldGpioPortResetPin(struct GpioHal *hal, uint32_t pins)
{
	hal->gpio->BSRR = pins << 16;
}

static inline void LldGpioPortTogglePin(struct GpioHal *hal, uint32_t pins)
{
	hal->gpio->ODR ^= pins;
}

static inline uint32_t LldGpioPortGetOutput(struct GpioHal *hal, uint32_t pins)
{
	return hal->gpio->ODR & pins;
}


static inline uint32_t LldGpioPortGetInput(struct GpioHal *hal, uint32_t pins)
{
	return hal->gpio->IDR & pins;
}






#endif
