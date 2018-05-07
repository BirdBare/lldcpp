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
args = args;
}

int main(void)
{
	uint32_t dmapins[50] = {0};
	uint32_t waste[50];

	DmaInterrupt dma_transfer(DMA2S3_HAL);
	dma_transfer.Settings().DataSize(32);
	dma_transfer.Init();
	uint32_t pins = GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
	dma_transfer.MemSet(dmapins,&pins,50);
	//dma sets correct pins as a test

	dma_transfer.Wait();

	GpioOutput GPIOD_OUT(
		GPIOD_HAL, 
		dmapins[49]);
	GPIOD_OUT.Init();
	//gpio stuff

	SpiDma spi(SPI1_HAL,1,1);
	spi.SetCallback(&call,0);
	spi.Settings().Master(true).DataSize(16).CrcPolynomial(1);

		GpioAlt spi_pins(
		GPIOA_HAL, 
		GPIO_PIN_5 | GPIO_PIN_7);
	//spi gpio stuff
	spi_pins.Settings().PuPd(GPIO_PUPD_PD).Alt(GPIO_ALT_5);
	spi_pins.Init();

	spi.Init();

while(1)
{
 spi.Transfer(&dmapins[0],waste,100);

 GPIOD_OUT.Toggle();
 //gpio



 for(int i = 0; i < 160000; i++)
 {
	WAIT;
 }


}



return 0;
}

