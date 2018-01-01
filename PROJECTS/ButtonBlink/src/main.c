//
//
//
//
//


#include "main.h"
#include "gpio_hal.h"
#include "spi_hal.h"
#include "nvic_lld.h"
#include "nokia5110.h"
#include "timer_hal.h"

void NMI_Handler(void)
{
BREAK(99);
}

void HardFault_Handler(void)
{
BREAK(98);
}

void MemManage_Handler(void)
{
BREAK(97);
}

void BusFault_Handler(void)
{
BREAK(96);
}

void UsageFault_Handler(void)
{
BREAK(95);
}


#include "bareos.h"
















uint8_t thread3_mem[500];

void thread3(void *args)
{

	float counter = 1.5;

	while(1)
	{
		counter *= (0.0-1.0) / (float)BareOsTimerGetTime();
		
		if(counter > (float)(1<<32 - 1))
			counter = 0;
	}
}






uint8_t blink_memory[500]; 

void blink(void *args)
{
	uint8_t data[6] =
		{0b10000001,0b10000001,0b10000001,0b10000001,0b10000001,0b10000001};
	
	float count = 0.0;

	while(1)
	{
		nokia.nokia_pins ^= 0b1 << LIGHT_BIT;

			if(count >= 1.0)
			SpiTransferInterrupt(&SPI1_OBJECT,data,data,1);

			count += 0.1;

			BareOSTimerDelayPolled(1000);
	}
};












int main(void)
{

//
// ###################################SYSTEM INIT @@@#########################
//

	asm volatile("mrs r2, MSP");
	asm volatile("sub r2, #500");
	asm volatile("msr PSP, r2");
	asm volatile("mrs r1, CONTROL");
	asm volatile("mov r2, #0b10");
	asm volatile("orr r1, r2");
	asm volatile("msr CONTROL, r1");
	asm volatile("ISB");
	
	SCB->CPACR |= 0b1111 << 20;
	FPU->FPCCR |= 0b11 << 30;
	
	asm volatile("DSB");
	asm volatile("ISB");

	FlashEnableArt(&FLASH_OBJECT);
	struct FlashConfig flash_config = {5};
	FlashConfig(&FLASH_OBJECT,&flash_config);
	//Enable Art Controller and set wait states

	struct ClockConfig clock_config = {168000000,168000000,42000000,84000000};
	ClockConfig(&clock_config);
	//configure the cpu clocks

	BareOSSchedulerInit(100,0);

	struct BareOSThread *blink_thread =	
		BareOSThreadCreateThread(blink_memory,&blink,0,500);

	struct BareOSThread *thread3_p =	
		BareOSThreadCreateThread(thread3_mem,&thread3,0,500);

BareOSSchedulerAddThread(blink_thread);
BareOSSchedulerRemoveThread(blink_thread);
BareOSSchedulerAddThread(blink_thread);
BareOSSchedulerAddThread(thread3_p);


	struct TimerConfig system_config = {.clock_speed = 10000};
	BareOSTimerInit(&TIMER6_OBJECT,&system_config);
	BareOSTimerStart();
	//initalize system timer



//
//############################## END SYSTEM INIT @@@@@@@####################
//



	
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
	NvicEnableInterrupt(TIM6_DAC_IRQn);

	gpio_config.pin = PIN_6;
	gpio_config.mode = MODE_OUTPUT;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);

	GpioSetOutput(&GPIOA_OBJECT, PIN_6);

	gpio_config.pin = PIN_5 | PIN_7;
	gpio_config.mode = MODE_ALTERNATE;
	gpio_config.alternate = ALTERNATE_5;
	gpio_config.pupd = PUPD_PD;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config spi pins.
	//SPI EXPERIMENTAL

	SpiInit(&SPI1_OBJECT);
	//init spi

	struct SpiConfig spi_config = { .slave_gpio_object = &GPIOA_OBJECT, 
		.slave_gpio_pin = PIN_6, .clock_frequency = 300000,
		.interrupt = &Nokia5110Interrupt, .args = &nokia};

	SpiConfigMaster(&SPI1_OBJECT, &spi_config);
	//config spi1 for lowest clock speed and default settings



	while(1)
	{


		if(GpioGetInput(&GPIOA_OBJECT, PIN_0) != 0)
		{
			GpioToggleOutput(&GPIOD_OBJECT, PIN_13);
			BareOSTimerDelayPolled(1000);

		}
		//if input is pressed. blink LED
		else
		{
			GpioSetOutput(&GPIOD_OBJECT, PIN_13);
		}
		//if input is depressed. turn on LED

	}
	return 1;
}

