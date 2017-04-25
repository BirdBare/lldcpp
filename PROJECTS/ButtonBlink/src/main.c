//
//
//
//
//


#include "main.h"
#include "gpio_lld.h"

uint32_t hello = 1;

int main(void)
{

	RccEnableClock(&GPIOD_OBJECT.rcc);
	RccEnableClock(&GPIOA_OBJECT.rcc);
	//enable peripheral clock for GPIOA and GPIOD

	struct GpioConfig gpio_config;
	//pin config struct

	gpio_config.pins = GPIO_PIN_13;
	gpio_config.mode = GPIO_MODE_OUTPUT;
	gpio_config.type = GPIO_NO_USE;
	gpio_config.speed = GPIO_SPEED_VHIGH;
	gpio_config.pupd = GPIO_NO_USE;
	GpioConfig(&GPIOD_OBJECT, &gpio_config);
	//config Orange LED

	gpio_config.pins = GPIO_PIN_0;
	gpio_config.mode = GPIO_MODE_INPUT;
	gpio_config.type = GPIO_NO_USE;
	gpio_config.speed = GPIO_NO_USE;
	gpio_config.pupd = GPIO_NO_USE;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config push button as input for turning led on and off

	while(hello != 0)
	{
	 
		if(GpioGetInput(&GPIOA_OBJECT, GPIO_PIN_0))
		{
			GpioResetOutput(&GPIOD_OBJECT, GPIO_PIN_13);
		}
		//if input is pressed. turn off LED
		else
		{
			GpioSetOutput(&GPIOD_OBJECT, GPIO_PIN_13);
		}
		//if input is depressed. turn on LED
	}
	return 1;
}




