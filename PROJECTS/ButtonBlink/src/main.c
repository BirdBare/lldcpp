//
//
//
//
//


#include "main.h"
#include "gpio_lld.h"
#include "clock_lld.h"
#include "flash_lld.h"
#include "systick_lld.h"

int main(void)
{

	FlashEnableArt(&FLASH_OBJECT);
	//Enable Flash ART Accelerator

	struct FlashConfig flash_config = {5};
	FlashConfig(&FLASH_OBJECT,&flash_config);
	//Config Flash Wait States

	struct ClockConfig clock_config = CLOCKCONFIG_168MHZ; 
	ClockConfig(&clock_config);
	//Config Cpu Clock to use PLL at 168Mhz
	//AHB = 168Mhz, APB1 = 42Mhz, APB2 = 84Mhz

	SysTickUpdate();
	SysTickEnable();
	//Update SysTick load register for 1 MilliSecond then enable the timer

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




