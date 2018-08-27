//
//
//
//
//

#include "gpio_hal.hpp"

/*
//******************************************************************************
//	
//									GPIOConfig
//		return: 0		- Success
//						!0	- Fail -- Pins not set returned
//	
//******************************************************************************
uint32_t GpioConfig(struct GpioObject * const gpio_object,
	const struct GpioConfig * const gpio_config)
{
	MutexLock(&gpio_object->mutex);

	uint32_t pins = gpio_config->pin;

		uint32_t setpins = gpio_object->used_pins & pins;
		//and used pins and config pins to check collision

	if(setpins != 0)
	{
		MutexUnlock(&gpio_object->mutex);
		return setpins;
	}
	//Pins collide if not zero. So return colliding pins to user for resolution.

	gpio_object->used_pins |= pins;
	//add new pins to used pins

	uint32_t ret = LldGpioConfig(gpio_object, gpio_config);
	//config the pins

	MutexUnlock(&gpio_object->mutex);

	return ret;
}


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t GpioResetConfig(struct GpioObject * const gpio_object,
	const uint32_t gpio_pin)
{
	MutexLock(&gpio_object->mutex);

	gpio_object->used_pins &= ~gpio_pin;
	//remove pin from used pins

	if(gpio_object->used_pins == 0)
	{
		LldGpioResetConfig(gpio_object);
	}

	MutexUnlock(&gpio_object->mutex);

	return 0;
}
*/


