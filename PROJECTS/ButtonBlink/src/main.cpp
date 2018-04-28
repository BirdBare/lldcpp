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
	SpiInterrupt spi(&SPI1_HAL);

	uint32_t dmapins[50] = {0};

	DmaInterrupt dma_transfer(&DMA2S3_HAL);
	dma_transfer.SetCallback(&call,0);
	dma_transfer.MemSet(dmapins,GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15,50);
	//dma sets correct pins as a test

		GpioAlt spi_pins(
		&GPIOA_HAL, 
		GPIO_PIN_5 | GPIO_PIN_7, 
		GPIO_ALT_5, 
		GPIO_TYPE_PUSHPULL, 
		GPIO_PUPD_PD);
	//spi gpio stuff

	
	uint16_t waste = 0b001100;
GpioOutput GPIOD_OUT(
		&GPIOD_HAL, 
		dmapins[0], 
		GPIO_TYPE_PUSHPULL, 
		GPIO_PUPD_OFF);
	//gpio stuff

uint8_t data = 0b10010010;


while(1)
{
 spi.Transfer(&data,&data,1);

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

