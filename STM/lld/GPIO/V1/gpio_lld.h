//
//
//
//
//



#ifndef GPIO_H
#define GPIO_H

#include "board.h"
#include "bare_defines.h"
#include "rcc_lld.h"

struct GpioObject
{
	const struct RccObject rcc; //clock object for clock register and bit location
	
	uint16_t pins; //used pins on this gpio port

	volatile GPIO_TypeDef * const gpio; //gpio pointer to gpio register base
};

extern struct GpioObject
	GPIOA_OBJECT,
	GPIOB_OBJECT,
	GPIOC_OBJECT,
	GPIOD_OBJECT,
	GPIOE_OBJECT,
	GPIOF_OBJECT,
	GPIOG_OBJECT,
	GPIOH_OBJECT,
	GPIOI_OBJECT,
	GPIOJ_OBJECT,
	GPIOK_OBJECT;

struct GpioConfig
{
	union
	{
		struct
		{
			uint16_t pins; //pins to set with the below configuration
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

//##############################################################################
//if member is not used. then use GPIO_NO_USE to set it to zero
#define GPIO_NO_USE ((uint32_t)0x00)

			uint8_t mode; //mode of the pins. How the pin functions
#define GPIO_MODE_INPUT ((uint32_t)0b00)
#define GPIO_MODE_OUTPUT ((uint32_t)0b01)
#define GPIO_MODE_ALTERNATE ((uint32_t)0b10)
#define GPIO_MODE_ANALOG ((uint32_t)0b11) 

			uint8_t type; //type of connection to voltage and ground
#define GPIO_TYPE_PUSHPULL ((uint32_t)0b0)
#define GPIO_TYPE_OPENDRAIN ((uint32_t)0b1)
			
			uint8_t speed; //switching speed of the pin
#define GPIO_SPEED_LOW ((uint32_t)0b00)
#define GPIO_SPEED_MED ((uint32_t)0b01)
#define GPIO_SPEED_HIGH ((uint32_t)0b10)
#define GPIO_SPEED_VHIGH ((uint32_t)0b11)

			uint8_t pupd; //if a pullup or pulldown is needed on the pin
#define GPIO_PUPD_PU ((uint32_t)0b01)
#define GPIO_PUPD_PD ((uint32_t)0b10)
			
			uint8_t alternate; //alternate function of pin for connecting UART, etc.
#define GPIO_ALTERNATE_0 ((uint32_t)0x00)
#define GPIO_ALTERNATE_1 ((uint32_t)0x01)
#define GPIO_ALTERNATE_2 ((uint32_t)0x02)
#define GPIO_ALTERNATE_3 ((uint32_t)0x03)
#define GPIO_ALTERNATE_4 ((uint32_t)0x04)
#define GPIO_ALTERNATE_5 ((uint32_t)0x05)
#define GPIO_ALTERNATE_6 ((uint32_t)0x06)
#define GPIO_ALTERNATE_7 ((uint32_t)0x07)
#define GPIO_ALTERNATE_8 ((uint32_t)0x08)
#define GPIO_ALTERNATE_9 ((uint32_t)0x09)
#define GPIO_ALTERNATE_10 ((uint32_t)0x0A)
#define GPIO_ALTERNATE_11 ((uint32_t)0x0B)
#define GPIO_ALTERNATE_12 ((uint32_t)0x0C)
#define GPIO_ALTERNATE_13 ((uint32_t)0x0D)
#define GPIO_ALTERNATE_14 ((uint32_t)0x0E)
#define GPIO_ALTERNATE_15 ((uint32_t)0x0F)

			uint8_t padding; //unused. To keep structure 32 bit aligned
		};
	};
};

//****** GPIO ********



//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t GpioConfig(
	struct GpioObject * const gpio_object,
	const struct GpioConfig * const gpio_config);



//******************************************************************************
//	
//										 
//	
//******************************************************************************
#define GPIO_RESET_CONFIG_USED 1

ALWAYS_INLINE uint32_t GpioResetPeripheral(
	struct GpioObject * const gpio_object) 
{
	if(gpio_object->pins == 0)
	{
		RccResetPeripheral(&(gpio_object->rcc));
		//call the clock reset function to set periph to reset values
		return 0;
	}
	//only run if no pins are used

	return GPIO_RESET_CONFIG_USED;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void GpioResetConfig(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin) 
{
	gpio_object->pins &= ~gpio_pin;
	//remove pin from used pins
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void GpioSetOutput(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin) 
{
	gpio_object->gpio->BSRR = (gpio_pin);
	//set pin atomically in Bit Set Reset Register
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void GpioResetOutput(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin) 
{
	gpio_object->gpio->BSRR = ((gpio_pin) << 16);
	//reset pin atomically in Bit Set Reset Register
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void GpioChangeOutput(
	struct GpioObject * const gpio_object, 
	const uint32_t set_gpio_pin, 
	const uint32_t reset_gpio_pin) 
{
	gpio_object->gpio->BSRR = (set_gpio_pin) | ((reset_gpio_pin) << 16);
	//set and reset pins atomically in Bit Set Reset Register
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void GpioToggleOutput(
	struct GpioObject * const gpio_object,
	const uint32_t gpio_pin)
{
	gpio_object->gpio->ODR ^= gpio_pin;
	//toggle output using XOR read modify write on Output Data Register
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t GpioGetOutput(
	struct GpioObject * const gpio_object,
	const uint32_t gpio_pin)
{
	return (uint16_t)gpio_object->gpio->ODR & gpio_pin;
	//read output pin from Output Data Register
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t GpioGetInput(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin)
{
	return (uint16_t)gpio_object->gpio->IDR & gpio_pin;
	//read input pin from Input Data Register
}



#endif
