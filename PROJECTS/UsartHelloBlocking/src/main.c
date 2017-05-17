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

int main(void)
{
	STARTUP();
	//start Systick, Set CPU clock at 168Mhz

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

	RccEnableClock(&UART4_OBJECT.rcc);
	//enable UART4 clock

	struct UsartConfig usart_config = {0};
	usart_config.baud_rate = 9600;
	usart_config.te = 1;
	//Basic Uart Transmitter Config

	UsartConfig(&UART4_OBJECT, &usart_config);
	//Config Uart

	struct CommunicationConfig t = {0,"hello",6,0-1};

	while(1)
		UsartPut8Blocking(&UART4_OBJECT,&t);
		//transmit hello with no timeout

	return 1;
}




