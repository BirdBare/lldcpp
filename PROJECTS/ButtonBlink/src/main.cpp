//
//
//
//
//

#include "main.hpp"
#include "gpio_lld.hpp"
#include "dma_lld.hpp"
#include "spi_lld.hpp"


extern "C" void __cxa_pure_virtual() {while(1) NOP;}

void call(void *args)
{
}

int main(void)
{

	uint32_t dmapins[50] = {0};

	DmaInterrupt dma_transfer(&DMA2S3_HAL);
	dma_transfer.MemSet(dmapins,GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15,50);
	//dma sets correct pins as a test

		GpioAlt spi_pins(
		&GPIOA_HAL, 
		GPIO_PIN_5 | GPIO_PIN_7, 
		GPIO_ALT_5, 
		GPIO_TYPE_PUSHPULL, 
		GPIO_PUPD_PD);
	//spi gpio stuff

	SpiPolled spi(&SPI1_HAL);
	//spi stuff
	
	uint16_t waste = 0b001100;
GpioOutput GPIOD_OUT(
		&GPIOD_HAL, 
		dmapins[4], 
		GPIO_TYPE_PUSHPULL, 
		GPIO_PUPD_OFF);
	//gpio stuff


while(1)
{

 GPIOD_OUT.Toggle();
 //gpio

	spi.Transmit(&waste,2);
	//spi

 for(int i = 0; i < 160000; i++)
 {
	WAIT;
 }

;

}



return 0;
}

