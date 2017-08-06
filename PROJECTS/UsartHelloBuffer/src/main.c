//
//
//
//
//


#include "main.h"
#include "gpio_lld.h"
#include "usart_lld.h"

//UART4 TX PA0
//UART4 RX PA1


	uint8_t tx_buffer_mem[256];

int main(void)
{
	FlashEnableArt(&FLASH_OBJECT);
	struct FlashConfig flash_config = {5};
	FlashConfig(&FLASH_OBJECT, &flash_config);
	//Enable art accelerator and change Flash wait ticks for increased clock speed

	struct ClockConfig clock_config = CLOCKCONFIG_168MHZ;
	ClockConfig(&clock_config);
	//Set CPU clock at 168Mhz

	RccEnableClock(&GPIOA_OBJECT.rcc);
	//Enable GPIOA clock


	struct GpioConfig gpio_config = {0};
	gpio_config.pin = PIN_0;
	gpio_config.mode = MODE_ALTERNATE;
	gpio_config.speed = SPEED_VHIGH;
	gpio_config.alternate = ALTERNATE_8;
	//set GPIOA pin0 as UART4 tx pin

	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config the pin

	UsartInitInterrupt(&UART4_OBJECT, &tx_buffer_mem, 255, 0, 0);
	//initialize the UART. 

	NvicEnableInterrupt(UART4_IRQn);
	//enable the interrupt for the txeie interrupt





while(1)
{
		UsartWriteBuffer(&UART4_OBJECT,(uint8_t *)"Hello\n",6);
}
	return 1;
}



