//
//
//
//
//

#include "main.hpp"
#include "gpio_lld.hpp"


int main(void)
{
	GpioOutput GPIOD_OUT(&GPIOD_HAL);


	GPIOD_OUT.Init();

	GPIOD_OUT.AddPins(GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15);

	GPIOD_OUT.Config();

	GPIOD_OUT.Set();

while(1)
{

 GPIOD_OUT.Toggle();

 for(int i = 0; i < 160000; i++)
 {
	WAIT;
 }

;

}



return 0;
}

