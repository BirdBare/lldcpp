//
//
//
//
//



#ifndef STM32F767_GPIO_H
#define STM32F767_GPIO_H

#include "board.h"
#include "BARE_DEFINES.h"
#include "RCC_lld.h"
#include <stdint.h>

struct GPIOxo
{
	struct RCCxo RCCo;
	
	uint16_t used;

	volatile GPIO_TypeDef * const GPIOx;
};

extern struct GPIOxo
	GPIOAo,
	GPIOBo,
	GPIOCo,
	GPIODo,
	GPIOEo,
	GPIOFo,
	GPIOGo,
	GPIOHo,
	GPIOIo,
	GPIOJo,
	GPIOKo;

struct GPIOCONFIG
{
	union
	{
		struct
		{
			uint16_t pins;
			uint8_t mode;
			uint8_t type;
			uint8_t speed;
			uint8_t pupd;
			uint8_t alt;
			uint8_t padding; //unused. To keep structure 32 bit aligned
		};
	};
};

//****** GPIO ********

#define GPIO_PIN_0         ((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_PIN_1         ((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_PIN_2         ((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_PIN_3         ((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_PIN_4         ((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_PIN_5         ((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_PIN_6         ((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_PIN_7         ((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_PIN_8         ((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_PIN_9         ((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_PIN_10        ((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_PIN_11        ((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_PIN_12        ((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_PIN_13        ((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_PIN_14        ((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_PIN_15        ((uint16_t)0x8000)  /* Pin 15 selected */
#define GPIO_PIN_ALL       ((uint16_t)0xFFFF)  /* All pins selected */

#define GPIO_NO_USE ((uint32_t)0x00)

#define GPIO_MODE_INPUT ((uint32_t)0b00)
#define GPIO_MODE_OUTPUT ((uint32_t)0b01)
#define GPIO_MODE_ALTERNATE ((uint32_t)0b10)
#define GPIO_MODE_ANALOG ((uint32_t)0b11) 

#define GPIO_OUTTYPE_PUSHPULL ((uint32_t)0b0)
#define GPIO_OUTTYPE_OPENDRAIN ((uint32_t)0b1)

#define GPIO_OUTSPEED_LOW ((uint32_t)0b00)
#define GPIO_OUTSPEED_MED ((uint32_t)0b01)
#define GPIO_OUTSPEED_HIGH ((uint32_t)0b10)
#define GPIO_OUTSPEED_VHIGH ((uint32_t)0b11)

#define GPIO_PUPD_OFF ((uint32_t)0b00)
#define GPIO_PUPD_PU ((uint32_t)0b01)
#define GPIO_PUPD_PD ((uint32_t)0b10)

#define GPIO_ALTFUNCTION_OFF ((uint32_t)0x00)
#define GPIO_ALTFUNCTION_0 ((uint32_t)0x00)
#define GPIO_ALTFUNCTION_1 ((uint32_t)0x01)
#define GPIO_ALTFUNCTION_2 ((uint32_t)0x02)
#define GPIO_ALTFUNCTION_3 ((uint32_t)0x03)
#define GPIO_ALTFUNCTION_4 ((uint32_t)0x04)
#define GPIO_ALTFUNCTION_5 ((uint32_t)0x05)
#define GPIO_ALTFUNCTION_6 ((uint32_t)0x06)
#define GPIO_ALTFUNCTION_7 ((uint32_t)0x07)
#define GPIO_ALTFUNCTION_8 ((uint32_t)0x08)
#define GPIO_ALTFUNCTION_9 ((uint32_t)0x09)
#define GPIO_ALTFUNCTION_10 ((uint32_t)0x0A)
#define GPIO_ALTFUNCTION_11 ((uint32_t)0x0B)
#define GPIO_ALTFUNCTION_12 ((uint32_t)0x0C)
#define GPIO_ALTFUNCTION_13 ((uint32_t)0x0D)
#define GPIO_ALTFUNCTION_14 ((uint32_t)0x0E)
#define GPIO_ALTFUNCTION_15 ((uint32_t)0x0F)

uint32_t GPIO_Config(volatile struct GPIOxo * const GPIOo, uint32_t GPIO_PIN,
	const uint32_t GPIO_MODE, const uint32_t GPIO_OUTTYPE,
	const uint32_t GPIO_OUTSPEED, const uint32_t GPIO_PUPD,
	const uint32_t GPIO_ALTFUNCTION);


#define GPIO_CONFIG_USED 1

ALWAYS_INLINE uint32_t GPIO_ResetConfig(volatile struct GPIOxo * const GPIOo, 
	uint32_t GPIO_PIN) 
{
	if(GPIOo->used == 0)
	{
		RCC_Reset((struct RCCxo *)GPIOo);
		return 0;
	}
	return GPIO_CONFIG_USED;
}

ALWAYS_INLINE void GPIO_Disable(volatile struct GPIOxo * const GPIOo, 
	uint32_t GPIO_PIN) 
{
	GPIOo->used &= ~GPIO_PIN;
}

ALWAYS_INLINE void GPIO_SetOutput(struct GPIOxo *GPIOo, uint32_t GPIO_PIN) 
{
	GPIOo->GPIOx->BSRR = (GPIO_PIN);
}

ALWAYS_INLINE void GPIO_ResetOutput(struct GPIOxo *GPIOo, uint32_t GPIO_PIN) 
{
	GPIOo->GPIOx->BSRR = ((GPIO_PIN) << 16);
}

ALWAYS_INLINE void GPIO_ChangeOutput(struct GPIOxo *GPIOo, 
	uint32_t setGPIO_PIN, uint32_t resetGPIO_PIN) 
{
	GPIOo->GPIOx->BSRR = (setGPIO_PIN) | ((resetGPIO_PIN) << 16);
}

ALWAYS_INLINE void GPIO_ToggleOutput(struct GPIOxo *GPIOo, uint32_t GPIO_PIN){
	GPIOo->GPIOx->ODR ^= GPIO_PIN;
}

ALWAYS_INLINE uint32_t GPIO_GetInState(struct GPIOxo *GPIOo) 
{
	return (uint16_t)GPIOo->GPIOx->IDR;
}


ALWAYS_INLINE uint32_t GPIO_GetOutState(struct GPIOxo *GPIOo)
{
	return (uint16_t)GPIOo->GPIOx->ODR;
}


#endif
