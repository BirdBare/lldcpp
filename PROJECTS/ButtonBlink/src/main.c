//
//
//
//
//


#include "main.h"
#include "gpio_lld.h"



int main(void)
{

	STARTUP();

	RccEnableClock(&GPIOD_OBJECT.rcc);
	RccEnableClock(&GPIOA_OBJECT.rcc);
	//enable peripheral clock for GPIOA and GPIOD

	struct GpioConfig gpio_config = {0};
	//pin config struct

	gpio_config.pins = GPIO_PIN_13;
	gpio_config.mode = GPIO_MODE_OUTPUT;
	gpio_config.speed = GPIO_SPEED_VHIGH;
	GpioConfig(&GPIOD_OBJECT, &gpio_config);
	//config Orange LED

	gpio_config.pins = GPIO_PIN_0;
	gpio_config.mode = GPIO_MODE_INPUT;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config push button as input for turning led on and off

	while(1)
	{
	 
		if(GpioGetInput(&GPIOA_OBJECT, GPIO_PIN_0) != 0)
		{
				GpioToggleOutput(&GPIOD_OBJECT, GPIO_PIN_13);
				SysTickDelayMilli(1000);
		}
		//if input is pressed. blink LED
		else
		{
			GpioSetOutput(&GPIOD_OBJECT, GPIO_PIN_13);
		}
		//if input is depressed. turn on LED
	}
	return 1;
}




