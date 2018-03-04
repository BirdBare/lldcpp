//
//
//
//
//



#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include "gpio_lld.h"
#include "bareos.h"
#include "mutex.h"


//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t GpioInit(struct GpioObject * const gpio_object)
{
	BareOSDisableInterrupts();

	if(gpio_object->initialized != 0)
	{	
		BareOSEnableInterrupts();
		return 1;
	}

	if(LldGpioInit(gpio_object) != 0)
	{
		BareOSEnableInterrupts();
		return 1;
	}

	LldGpioResetConfig(gpio_object);

	MutexInit(&gpio_object->mutex);
	
	gpio_object->initialized = 1;
	
	BareOSEnableInterrupts();

	return 0;
}

static inline uint32_t GpioDeinit(struct GpioObject * const gpio_object)
{

	BareOSDisableInterrupts();

	if(LldGpioDeinit(gpio_object) != 0)
	{
		BareOSEnableInterrupts();
		return 1;
	}

	MutexDeinit(&gpio_object->mutex);

	gpio_object->initialized = 0;
	
	BareOSEnableInterrupts();

	return 0;
}

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
uint32_t GpioResetConfig(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin); 



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t GpioSetOutput(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin) 
{
	MutexLock(&gpio_object->mutex);
	uint32_t ret = LldGpioSetOutput(gpio_object, gpio_pin);	
	MutexUnlock(&gpio_object->mutex);
	return ret;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t GpioResetOutput(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin) 
{
	MutexLock(&gpio_object->mutex);
	uint32_t ret = LldGpioResetOutput(gpio_object, gpio_pin);	
	MutexUnlock(&gpio_object->mutex);
	return ret;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t GpioChangeOutput(
	struct GpioObject * const gpio_object, 
	const uint32_t set_gpio_pin, 
	const uint32_t reset_gpio_pin) 
{
	MutexLock(&gpio_object->mutex);
	uint32_t ret = LldGpioChangeOutput(gpio_object, set_gpio_pin, reset_gpio_pin);	
	MutexUnlock(&gpio_object->mutex);
	return ret;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t GpioToggleOutput(
	struct GpioObject * const gpio_object,
	const uint32_t gpio_pin)
{
	MutexLock(&gpio_object->mutex);
	uint32_t ret = LldGpioToggleOutput(gpio_object, gpio_pin);	
	MutexUnlock(&gpio_object->mutex);
	return ret;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t GpioGetOutput(
	struct GpioObject * const gpio_object,
	const uint32_t gpio_pin,
	uint32_t *output_data)
{
	MutexLock(&gpio_object->mutex);
	uint32_t ret = LldGpioGetOutput(gpio_object, gpio_pin, output_data);
	MutexUnlock(&gpio_object->mutex);
	return ret;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t GpioGetInput(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin,
	uint32_t *input_data)
{
	MutexLock(&gpio_object->mutex);
	uint32_t ret = LldGpioGetInput(gpio_object, gpio_pin, input_data);
	MutexUnlock(&gpio_object->mutex);
	return ret;
}



#endif
