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

	gpio_object->initialized = 1;
	
	LldGpioInit(gpio_object);	
	MutexInit(&gpio_object->mutex);
	BareOSEnableInterrupts();

	return 0;
}

static inline void GpioDeinit(struct GpioObject * const gpio_object)
{
	gpio_object->initialized = 0;

	BareOSDisableInterrupts();
	LldGpioDeinit(gpio_object);	
	BareOSEnableInterrupts();
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
void GpioResetConfig(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin); 



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void GpioSetOutput(
	struct GpioObject * const gpio_object, 
	const uint32_t gpio_pin) 
{
	MutexLock(&gpio_object->mutex);
	LldGpioSetOutput(gpio_object, gpio_pin);	
	MutexUnlock(&gpio_object->mutex);
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
	LldGpioResetOutput(gpio_object, gpio_pin);	
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
	LldGpioChangeOutput(gpio_object, set_gpio_pin, reset_gpio_pin);	
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
	LldGpioToggleOutput(gpio_object, gpio_pin);	
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
	return LldGpioGetOutput(gpio_object, gpio_pin);
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
	return LldGpioGetInput(gpio_object, gpio_pin);
}



#endif
