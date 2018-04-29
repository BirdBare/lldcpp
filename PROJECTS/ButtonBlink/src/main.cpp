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
	SpiSettings spisettings;
	spisettings.Master(true).DataSize(2);

	SpiDma spi(SPI1_HAL,spisettings);

	uint32_t dmapins[50] = {0};

	DmaSettings dsettings;

	DmaInterrupt dma_transfer(DMA2S3_HAL, dsettings);
	dma_transfer.SetCallback(&call,0);
	dma_transfer.MemSet(dmapins,GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15,50);
	//dma sets correct pins as a test

	GpioSettings gsettings;
	gsettings.Alt(GPIO_ALT_5).PuPd(GPIO_PUPD_PD);

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
uint32_t waste[50];

while(1)
{
 spi.Transfer(&dmapins[0],&waste[0],50);

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

