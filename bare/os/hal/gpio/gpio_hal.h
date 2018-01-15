//
//
//
//
//



#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include "gpio_lld.h"
#include "bareos.h"

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
	BareOSDisableInterrupts();
	LldGpioSetOutput(gpio_object, gpio_pin);	
	BareOSEnableInterrupts();
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
	BareOSDisableInterrupts();
	LldGpioResetOutput(gpio_object, gpio_pin);	
	BareOSEnableInterrupts();
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
	BareOSDisableInterrupts();
	LldGpioChangeOutput(gpio_object, set_gpio_pin, reset_gpio_pin);	
	BareOSEnableInterrupts();
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
	BareOSDisableInterrupts();
	LldGpioToggleOutput(gpio_object, gpio_pin);	
	BareOSEnableInterrupts();
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
