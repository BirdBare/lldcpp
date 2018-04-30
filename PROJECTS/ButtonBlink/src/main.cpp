//
//
//
//
//

#include "main.hpp"
#include "gpio_lld.hpp"
#include "dma_lld.hpp"
#include "spi_lld.hpp"



void call(void *args)
{
	((DmaInterrupt *)args)->Unlock();
	//unlock dma in callback
}

int main(void)
{
	uint32_t dmapins[50] = {0};
	uint32_t waste[50];

	DmaInterrupt dma_transfer(DMA2S3_HAL);
	dma_transfer.Lock();
	dma_transfer.SetCallback(&call,&dma_transfer);
	dma_transfer.MemSet(dmapins,GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15,50);
	//dma sets correct pins as a test

	SpiSettings spisettings;
	spisettings.Master(true).DataSize(16);

	SpiDma spi(SPI1_HAL,spisettings);

	GpioOutput GPIOD_OUT(
		GPIOD_HAL, 
		dmapins[0]);
	//gpio stuff

	GpioSettings gsettings;
	gsettings.Alt(GPIO_ALT_5).PuPd(GPIO_PUPD_PD);

		GpioAlt spi_pins(
		GPIOA_HAL, 
		GPIO_PIN_5 | GPIO_PIN_7,
		gsettings);
	//spi gpio stuff

while(1)
{
 GPIOD_OUT.Toggle();
 //gpio



 for(int i = 0; i < 160000; i++)
 {
	WAIT;
 }

;

}



return 0;
}

