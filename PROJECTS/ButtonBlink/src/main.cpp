//
//
//
//
//

#include "main.hpp"
#include "gpio_hal.hpp"
//#include "dma_lld.hpp"
//#include "spi_dma.hpp"
//#include "spi_interrupt.hpp"
























void call(void *args)
{
args = args;
}

int main(void)
{
	uint32_t pins = GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;


	GpioOutput GPIOD_OUT(GPIOD_PORT);
	GPIOD_OUT.Settings().Pins(pins);
	GPIOD_OUT.Init();
	GPIOD_OUT.Config();
	//gpio stuff




while(1)
{

 GPIOD_OUT.Toggle(GPIO_PIN_ALL);
 //gpio



 for(int i = 0; i < 160000; i++)
 {
	WAIT;
 }


}


while(1);

return 0;
}

