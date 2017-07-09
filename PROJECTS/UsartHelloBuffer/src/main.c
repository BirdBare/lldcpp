//
//
//
//
//


#include "main.h"
#include "gpio_lld.h"
#include "usart_lld.h"
#include "nvic_lld.h"

//UART4 TX PA0
//UART4 RX PA1


	const uint8_t tx_buffer_mem[256];

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
	usart_config.clock_frequency = 9600;
	usart_config.te = 1;
	usart_config.txeie = 1;
	//Basic Uart Transmitter Config

	NvicEnableInterrupt(UART4_IRQn);
	//enable the interrupt for the txeie interrupt

	UsartConfig(&UART4_OBJECT, &usart_config);
	//Config Uart

	struct CommunicationConfig t = {"hello\n",0,6,0-1};

	UsartInit(&UART4_OBJECT, &tx_buffer_mem, 255, 0, 0);
	//initialize the UART. Soon will not need to be static


while(1)
{
		UsartWrite8Buffer(&UART4_OBJECT,&t);
		for(int i=0; i<1000000; i++)
asm volatile("nop");
}
	return 1;
}




