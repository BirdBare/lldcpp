//
//
//
//
//



#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include "gpio_lld.h"


class GpioBase
{
private:
  _GpioPort &_port;
  //object which is modified by lld code for hal wrapper functions
  
  _GpioSettings _settings;
  //gpio settings. User added settings are also here
  
protected:
  _GpioPort& Port(void)
  {
    return _port;
  } 
  
  GpioSettings& Settings(void)
  {
    return _settings;
  } 
  
  GpioBase(_GpioPort &port)
  : _port(port)
  {}
  
public:
  bool Init(void)
  {
		//BareOSDisableInterrupts();
    
		bool ret = LldGpioPortInit(Port());
	
		//BareOSEnableInterrupts();

		return ret;
  } 
};

class GpioOutput : public GpioBase
{
public:
  GpioOutput(GpioHal &hal) 
  : GpioBase(hal) 
  { 
  }
  //constructor for gpioOutput
	
	bool Config(void)
	{
		//BareOSDisableInterrupts();

		bool ret = LldGpioPortConfigOutput(Port(),Settings());

		//BareOSEnableInterrupts();

		return ret;
	}

  GpioOutput& Set(uint32_t pins = GPIO_PIN_ALL) 
  { 
		//BareOSDisableInterrupts();
		
		LldGpioPortSetPin(Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this; 
  } 

  GpioOutput& Toggle(uint32_t pins = GPIO_PIN_ALL) 
  { 
		//BareOSDisableInterrupts();

		LldGpioPortTogglePin(Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this; 
  } 

  GpioOutput& Reset(uint32_t pins = GPIO_PIN_ALL) 
  { 
		//BareOSDisableInterrupts();

		LldGpioPortResetPin(Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this;
  }
  //set toggle and reset pin or pins

	uint32_t Get(uint32_t pins = GPIO_PIN_ALL)
  { 
		//BareOSDisableInterrupts();

		uint32_t ret = LldGpioPortGetOutput(Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();

		return ret;
  }
  //get value of pin. set or reset


};


/*
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
*/


#endif
