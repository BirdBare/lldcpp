//
//
//
//
//


#include "main.h"
#include "gpio_lld.h"
#include "bare_timer.h"


int main(void)
{

	STARTUP();

	RccEnableClock(&GPIOD_OBJECT.rcc);
	RccEnableClock(&GPIOA_OBJECT.rcc);
	//enable peripheral clock for GPIOA and GPIOD

	struct GpioConfig gpio_config = {0};
	//pin config struct

	gpio_config.pin = PIN_13;
	gpio_config.mode = MODE_OUTPUT;
	gpio_config.speed = SPEED_VHIGH;
	GpioConfig(&GPIOD_OBJECT, &gpio_config);
	//config Orange LED

	gpio_config.pin = PIN_0;
	gpio_config.mode = MODE_INPUT;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config push button as input for turning led on and off

	while(1)
	{
	 
		if(GpioGetInput(&GPIOA_OBJECT, PIN_0) != 0)
		{
				GpioToggleOutput(&GPIOD_OBJECT, PIN_13);
				TimerDelayMilli(1000);
		}
		//if input is pressed. blink LED
		else
		{
			GpioSetOutput(&GPIOD_OBJECT, PIN_13);
		}
		//if input is depressed. turn on LED
	}
	return 1;
}




