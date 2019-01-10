//
//
//
//
//



#ifndef GPIO_HAL_H
#define GPIO_HAL_H

extern "C"
{
#include "gpio_lld.h"
}

#ifndef GPIO_EXTRA_SETTINGS_ACCESSORS
#define GPIO_EXTRA_SETTINGS_ACCESSORS
#endif

enum GPIO_PIN;
enum GPIO_TYPE;
enum GPIO_PUPD;
enum GPIO_ALT;
enum GPIO_INT;

class GpioBaseSettings
{
	GpioSettings_t _gpio_settings = {};
	
public:
	GpioSettings_t& GpioSettings(void)
	{
		return _gpio_settings;
	}

	//Set and Get Pins
  uint32_t Pins(void)
  {
    return GpioGetPins(&GpioSettings());
  }
	GpioBaseSettings& Pins(uint32_t pins)
  {
    GpioSetPins(&GpioSettings(),pins);
    return *this;
  }
  GpioBaseSettings& Pins(GPIO_PIN pin)
  {
    Pins((uint32_t)pin);
    return *this;
  }

  //Set and Get Output Type
  GPIO_TYPE Type(void)
  {
    return GpioGetType(&GpioSettings());
  }
  GpioBaseSettings& Type(GPIO_TYPE type)
  {
    GpioSetType(&GpioSettings(),type);
    return *this;
  }

  //Set and Get Pull up or Pull down
  GPIO_PUPD PuPd(void)
  {
    return GpioGetPuPd(&GpioSettings());
  }
  GpioBaseSettings& PuPd(GPIO_PUPD pupd)
  {
    GpioSetPuPd(&GpioSettings(),pupd);
    return *this;
  }

  //Set and Get Alternate Function
  GPIO_ALT Alt(void)
  {
    return GpioGetAlt(&GpioSettings());
  }
  GpioBaseSettings& Alt(GPIO_ALT alt)
  {
    GpioSetAlt(&GpioSettings(),alt);
    return *this;
  }

	GPIO_EXTRA_SETTINGS_ACCESSORS
};
//******************************************************************************
//
//
//
//******************************************************************************
class GpioBase
{
private:
  GpioPort_t &_port;
  //object which is modified by lld code for port wrapper functions
  
  class GpioBaseSettings _settings;
  //gpio settings. User added settings are also here
  
protected:
  GpioPort_t& Port(void)
  {
    return _port;
  } 
  
    GpioBase(GpioPort_t &port)
  : _port(port)
  {}
  
public:
  GpioBaseSettings& Settings(void)
  {
    return _settings;
  } 
  
	bool Init(void)
  {
		//BareOSDisableInterrupts();
    
		bool ret = GpioInit(&Port());
	
		//BareOSEnableInterrupts();

		return ret;
  } 

	bool Deinit(void)
  {
		//BareOSDisableInterrupts();
    
		bool ret = GpioDeinit(&Port());
	
		//BareOSEnableInterrupts();

		return ret;
  } 

	bool Deconfig(void)
  {
		//BareOSDisableInterrupts();
    
		bool ret = GpioDeconfig(&Port(),&Settings().GpioSettings());
	
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
  GpioOutput(GpioPort_t &port) 
  : GpioBase(port) 
  { 
  }
  //constructor for gpioOutput
	
	bool Config(void)
	{
		//BareOSDisableInterrupts();

		bool ret = GpioConfigOutput(&Port(),&Settings().GpioSettings());

		//BareOSEnableInterrupts();

		return ret;
	}

  GpioOutput& Set(uint32_t pins) 
  { 
		//BareOSDisableInterrupts();
		
		GpioSetPin(&Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this; 
  } 

  GpioOutput& Toggle(uint32_t pins) 
  { 
		//BareOSDisableInterrupts();

		GpioTogglePin(&Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this; 
  } 

  GpioOutput& Reset(uint32_t pins) 
  { 
		//BareOSDisableInterrupts();

		GpioResetPin(&Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this;
  }
  //set toggle and reset pin or pins

	uint32_t Get(uint32_t pins)
  { 
		//BareOSDisableInterrupts();

		uint32_t ret = GpioGetOutput(&Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();

		return ret;
  }
  //get value of pin. set or reset
};


class GpioInput : public GpioBase
{
public:
  GpioInput(GpioPort_t &port)
    : GpioBase(port)
    {}
  
	bool Config(void)
	{
		//BareOSDisableInterrupts();

		bool ret = GpioConfigInput(&Port(),&Settings().GpioSettings());

		//BareOSEnableInterrupts();

		return ret;
	}

	uint32_t Get(uint32_t pins)
  { 
		//BareOSDisableInterrupts();

		uint32_t ret = GpioGetInput(&Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();

		return ret;
  }
  //get value of pin. set or reset
};


class GpioAlt : public GpioBase
{
public:
  GpioAlt(GpioPort_t &port) 
  : GpioBase(port)
  {}
  //constructor for gpioAlternate

	bool Config(void)
	{
		//BareOSDisableInterrupts();

		bool ret = GpioConfigAlternate(&Port(),&Settings().GpioSettings());

		//BareOSEnableInterrupts();

		return ret;
	}
};


class GpioAnalog : public GpioBase
{
public:
  GpioAnalog(GpioPort_t &port) 
  : GpioBase(port) 
  {}  

  bool Config(void)
	{
		//BareOSDisableInterrupts();

		bool ret = GpioConfigAnalog(&Port(),&Settings().GpioSettings());

		//BareOSEnableInterrupts();

		return ret;
	}
};









#endif
