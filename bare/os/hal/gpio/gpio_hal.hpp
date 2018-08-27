//
//
//
//
//



#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include "gpio_lld.h"


//******************************************************************************
//
//
//
//******************************************************************************
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
  
    GpioBase(_GpioPort &port)
  : _port(port), _settings()
  {}
  
public:
  GpioSettings& Settings(void)
  {
    return _settings;
  } 
  
	bool Init(void)
  {
		//BareOSDisableInterrupts();
    
		bool ret = LldGpioPortInit(Port());
	
		//BareOSEnableInterrupts();

		return ret;
  } 
};


//******************************************************************************
//
//
//
//******************************************************************************
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


class GpioInput : public GpioBase
{
public:
  GpioInput(GpioHal &hal)
    : GpioBase(hal)
    {}
  
	bool Config(void)
	{
		//BareOSDisableInterrupts();

		bool ret = LldGpioPortConfigInput(Port(),Settings());

		//BareOSEnableInterrupts();

		return ret;
	}

	uint32_t Get(uint32_t pins = GPIO_PIN_ALL)
  { 
		//BareOSDisableInterrupts();

		uint32_t ret = LldGpioPortGetInput(Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();

		return ret;
  }
  //get value of pin. set or reset
};


class GpioAlt : public GpioBase
{
public:
  GpioAlt(GpioHal &hal) 
  : GpioBase(hal)
  {}
  //constructor for gpioAlternate

	bool Config(void)
	{
		//BareOSDisableInterrupts();

		bool ret = LldGpioPortConfigAlternate(Port(),Settings());

		//BareOSEnableInterrupts();

		return ret;
	}
};


class GpioAnalog : public GpioBase
{
public:
  GpioAnalog(GpioHal &hal) 
  : GpioBase(hal) 
  {}  

  bool Config(void)
	{
		//BareOSDisableInterrupts();

		bool ret = LldGpioPortConfigAnalog(Port(),Settings());

		//BareOSEnableInterrupts();

		return ret;
	}
};









#endif
