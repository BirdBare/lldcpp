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
}

int main(void)
{
	SpiInterrupt spi(SPI1_HAL);

	uint32_t dmapins[50] = {0};

	DmaSettings settings;

	DmaInterrupt dma_transfer(DMA2S3_HAL, settings);
	dma_transfer.SetCallback(&call,0);
	dma_transfer.MemSet(dmapins,GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15,50);
	//dma sets correct pins as a test

	GpioSettings gsettings;
	gsettings.Alt(GPIO_ALT_5);

		GpioAlt spi_pins(
		GPIOA_HAL, 
		GPIO_PIN_5 | GPIO_PIN_7,
		gsettings);
	//spi gpio stuff

	
GpioOutput GPIOD_OUT(
		GPIOD_HAL, 
		dmapins[0]);
	//gpio stuff

uint8_t data = 0b10010010;
uint8_t waste;

while(1)
{
 spi.Transfer(&data,&waste,1);

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

