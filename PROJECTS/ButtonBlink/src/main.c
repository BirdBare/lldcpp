//
//
//
//
//


#include "main.h"
#include "gpio_lld.h"
#include "spi_lld.h"
#include "nvic_lld.h"

int main(void)
{
	FlashEnableArt(&FLASH_OBJECT);
	struct FlashConfig flash_config = {5};
	FlashConfig(&FLASH_OBJECT,&flash_config);
	//Enable Art Controller and set wait states

	struct ClockConfig clock_config = {168000000,168000000,42000000,84000000};
	ClockConfig(&clock_config);
	//configure the cpu clocks

	RccEnableClock(&GPIOD_OBJECT.rcc);
	RccEnableClock(&GPIOA_OBJECT.rcc);
	//enable peripheral clock for GPIOA and GPIOD

	

	struct GpioConfig gpio_config = {0};
	//pin config struct

	gpio_config.pin = PIN_13;
	gpio_config.mode = MODE_OUTPUT;
	gpio_config.speed = SPEED_VHIGH;
	GpioConfig(&GPIOD_OBJECT, &gpio_config);
	//config Orange LED

	gpio_config.pin = PIN_0;
	gpio_config.mode = MODE_INPUT;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config push button as input for turning led on and off

	
	NvicEnableInterrupt(SPI1_IRQn);

	gpio_config.pin = PIN_5 | PIN_6 | PIN_7;
	gpio_config.mode = MODE_ALTERNATE;
	gpio_config.alternate = ALTERNATE_5;
	gpio_config.pupd = PUPD_PD;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config spi pins.
	//SPI EXPERIMENTAL

	RccEnableClock(&SPI1_OBJECT.rcc);
	//enable spi1

	struct SpiConfig spi_config = { .error_interrupt = 1, 
		.multimaster_disable = 1, .crc_polynomial = 0b11110110};
	SpiConfigMaster(&SPI1_OBJECT, &spi_config);
	//config spi1 for lowest clock speed and default settings

	RccEnableClock(&SPI1_OBJECT.tx_dma_object->rcc);
//DMA ENABLE

				uint8_t data[5] =
				{0b10000001,0b10000001,0b10000001,0b10000001,0b10000001};


	while(1)
	{
	 
		if(GpioGetInput(&GPIOA_OBJECT, PIN_0) != 0)
		{
				GpioToggleOutput(&GPIOD_OBJECT, PIN_13);

				for(int i = 0; i < 50000000; i++)
					asm("");
		}
		//if input is pressed. blink LED
		else
		{
			GpioSetOutput(&GPIOD_OBJECT, PIN_13);
		}
		//if input is depressed. turn on LED

				SpiTransmitDma(&SPI1_OBJECT,5,&data);

				for(int i = 0; i < 5000000; i++)
				 asm volatile ("nop");

	}
	return 1;
}

void SPI1_IRQHandler(void)
{


	BREAK(1);

}


