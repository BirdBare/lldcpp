//
//
//
//
//

#include "main.hpp"
#include "gpio_lld.hpp"
#include "dma_lld.hpp"


int main(void)
{
	DmaObject dma_transfer(&DMA2S3_HAL);

	uint32_t dmapins = 0;

	dma_transfer.MemSet(&dmapins,GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15,1);

	GpioOutput GPIOD_OUT(&GPIOD_HAL);

	GPIOD_OUT.AddPins(dmapins);

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

