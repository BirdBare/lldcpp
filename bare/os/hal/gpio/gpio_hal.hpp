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
	GpioSettings_t _user_settings = {};
	
public:
	GpioSettings_t& UserSettings(void)
	{
		return _user_settings;
	}

	//Set and Get Pins
  uint32_t Pins(void)
  {
    return GetPins(&UserSettings());
  }
	GpioBaseSettings& Pins(uint32_t pins)
  {
    SetPins(&UserSettings(),pins);
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
    return GetType(&UserSettings());
  }
  GpioBaseSettings& Type(GPIO_TYPE type)
  {
    SetType(&UserSettings(),type);
    return *this;
  }

  //Set and Get Pull up or Pull down
  GPIO_PUPD PuPd(void)
  {
    return GetPuPd(&UserSettings());
  }
  GpioBaseSettings& PuPd(GPIO_PUPD pupd)
  {
    SetPuPd(&UserSettings(),pupd);
    return *this;
  }

  //Set and Get Alternate Function
  GPIO_ALT Alt(void)
  {
    return GetAlt(&UserSettings());
  }
  GpioBaseSettings& Alt(GPIO_ALT alt)
  {
    SetAlt(&UserSettings(),alt);
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
  
	bool_t Init(void)
  {
		//BareOSDisableInterrupts();
    
		bool_t ret = LldGpioPortInit(&Port());
	
		//BareOSEnableInterrupts();

		return ret;
  } 

	bool_t Deinit(void)
  {
		//BareOSDisableInterrupts();
    
		bool_t ret = LldGpioPortDeinit(&Port());
	
		//BareOSEnableInterrupts();

		return ret;
  } 

	bool_t Deconfig(void)
  {
		//BareOSDisableInterrupts();
    
		bool_t ret = LldGpioPortDeconfig(&Port(),&Settings().UserSettings());
	
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
	
	bool_t Config(void)
	{
		//BareOSDisableInterrupts();

		bool_t ret = LldGpioPortConfigOutput(&Port(),&Settings().UserSettings());

		//BareOSEnableInterrupts();

		return ret;
	}

  GpioOutput& Set(uint32_t pins) 
  { 
		//BareOSDisableInterrupts();
		
		LldGpioPortSetPin(&Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this; 
  } 

  GpioOutput& Toggle(uint32_t pins) 
  { 
		//BareOSDisableInterrupts();

		LldGpioPortTogglePin(&Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this; 
  } 

  GpioOutput& Reset(uint32_t pins) 
  { 
		//BareOSDisableInterrupts();

		LldGpioPortResetPin(&Port(), pins & Settings().Pins());

		//BareOSEnableInterrupts();
    return *this;
  }
  //set toggle and reset pin or pins

	uint32_t Get(uint32_t pins)
  { 
		//BareOSDisableInterrupts();

		uint32_t ret = LldGpioPortGetOutput(&Port(), pins & Settings().Pins());

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
  
	bool_t Config(void)
	{
		//BareOSDisableInterrupts();

		bool_t ret = LldGpioPortConfigInput(&Port(),&Settings().UserSettings());

		//BareOSEnableInterrupts();

		return ret;
	}

	uint32_t Get(uint32_t pins)
  { 
		//BareOSDisableInterrupts();

		uint32_t ret = LldGpioPortGetInput(&Port(), pins & Settings().Pins());

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

	bool_t Config(void)
	{
		//BareOSDisableInterrupts();

		bool_t ret = LldGpioPortConfigAlternate(&Port(),&Settings().UserSettings());

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

  bool_t Config(void)
	{
		//BareOSDisableInterrupts();

		bool_t ret = LldGpioPortConfigAnalog(&Port(),&Settings().UserSettings());

		//BareOSEnableInterrupts();

		return ret;
	}
};









#endif
