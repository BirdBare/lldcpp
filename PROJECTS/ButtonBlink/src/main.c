//
//
//
//
//

#include "main.h"
#include "gpio_hal.h"
#include "spi_hal.h"
#include "dma_hal.h"
#include "nvic_lld.h"
#include "nokia5110.h"
#include "timer_hal.h"
#include "clock_hal.h"
#include "bareos.h"

void NMI_Handler(void)
{
BREAK(99);
}

void HardFault_Handler(void)
{

	asm("mrs r0, psp");
	asm("b HardFault_HandlerC");

	BREAK(79);

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

void HardFault_HandlerC(unsigned long *hardfault_args){
volatile unsigned long stacked_r0 ;
volatile unsigned long stacked_r1 ;
volatile unsigned long stacked_r2 ;
volatile unsigned long stacked_r3 ;
volatile unsigned long stacked_r12 ;
volatile unsigned long stacked_lr ;;
volatile unsigned long stacked_pc ;
volatile unsigned long stacked_psr ;
volatile unsigned long _CFSR ;
volatile unsigned long _HFSR ;
volatile unsigned long _DFSR ;
volatile unsigned long _AFSR ;
volatile unsigned long _BFAR ;
volatile unsigned long _MMAR ;

stacked_r0 = ((unsigned long)hardfault_args[0]) ;
stacked_r1 = ((unsigned long)hardfault_args[1]) ;
stacked_r2 = ((unsigned long)hardfault_args[2]) ;
	asm("");
stacked_r3 = ((unsigned long)hardfault_args[3]) ;
stacked_r12 = ((unsigned long)hardfault_args[4]) ;
stacked_lr = ((unsigned long)hardfault_args[5]) ;
stacked_pc = ((unsigned long)hardfault_args[6]) ;
stacked_psr = ((unsigned long)hardfault_args[7]) ;

_CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;
_HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;
_DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;
_AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;
_MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
_BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;
BREAK(5);

stacked_r0;
stacked_r1;
stacked_r2;
stacked_r3;
stacked_r12;
stacked_lr;
stacked_pc;
stacked_psr;

_CFSR;
_HFSR;
_DFSR;
_AFSR;
_MMAR;
_BFAR;

}









uint8_t blink1_mem[500];
void blink1(void *args)
{
GpioInit(&GPIOD_OBJECT);

	struct GpioConfig gpio_config = {0};
	//pin config struct

	gpio_config.pin = PIN_12;
	gpio_config.mode = MODE_OUTPUT;
	gpio_config.speed = SPEED_VHIGH;
	GpioConfig(&GPIOD_OBJECT, &gpio_config);
	//config Green LED

	float time = 1000.15444562;

	while(1)
	{
		time += (float) BareOSTimerGetTime() - 50.123552;
		
		if(time > 23297.05)
			GpioToggleOutput(&GPIOD_OBJECT, PIN_12);
		BareOSTimerDelayInterrupt(250);
	}
}



uint8_t blink2_mem[500];
void blink2(void *args)
{

GpioInit(&GPIOD_OBJECT);

	struct GpioConfig gpio_config = {0};
	//pin config struct

	gpio_config.pin = PIN_15;
	gpio_config.mode = MODE_OUTPUT;
	gpio_config.speed = SPEED_VHIGH;
	GpioConfig(&GPIOD_OBJECT, &gpio_config);
	//config Blue LED

	while(1)
	{
			GpioToggleOutput(&GPIOD_OBJECT, PIN_15);
		BareOSTimerDelayInterrupt(500);
	}
}


uint8_t blink3_mem[500]; 
void blink3(void *args)
{
	GpioInit(&GPIOD_OBJECT);

	struct GpioConfig gpio_config = {0};
	//pin config struct

	gpio_config.pin = PIN_14;
	gpio_config.mode = MODE_OUTPUT;
	gpio_config.speed = SPEED_VHIGH;
	GpioConfig(&GPIOD_OBJECT, &gpio_config);
	//config Red LED

	while(1)
	{
			GpioToggleOutput(&GPIOD_OBJECT, PIN_14);
			BareOSTimerDelayInterrupt(750);
	}
};




uint8_t data_out[50] = {0b10000001};

uint8_t spi_memory[500]; 
void spi(void *args)
{
	GpioInit(&GPIOA_OBJECT);

	struct GpioConfig gpio_config = {0};
	//pin config struct

	gpio_config.pin = PIN_6;
	gpio_config.mode = MODE_OUTPUT;
	gpio_config.speed = SPEED_VHIGH;
	//GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config nokia pin

	gpio_config.pin = PIN_6 | PIN_5 | PIN_7;
	gpio_config.mode = MODE_ALTERNATE;
	gpio_config.speed = SPEED_VHIGH;
	gpio_config.alternate = ALTERNATE_5;
	gpio_config.pupd = PUPD_PD;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config spi pins.

	SpiInit(&SPI1_OBJECT);
	//init spi

	struct SpiConfig spi_config = 
	{ .clock_frequency = 300000,
		.slave_gpio_object = &GPIOA_OBJECT,
		.slave_gpio_pin = PIN_6,
		.interrupt = 0,//&Nokia5110Interrupt,
		.interrupt_args = &nokia};

	SpiConfigMaster(&SPI1_OBJECT, &spi_config);
	//config spi1 for lowest clock speed and default settings

	while(1)
	{
		nokia.nokia_pins ^= 1 << LIGHT_BIT;

		SpiTransmitInterrupt(&SPI1_OBJECT,data_out,1);
		BareOSTimerDelayInterrupt(100);
	}
}







uint8_t main_memory[1024];
int main(void)
{
struct ClockConfig clock_config = 
	{168000000,
	168000000,
	42000000,
	84000000,
	8000000};
	ClockConfig(&clock_config);
	//configure the cpu clocks

	struct BareOSThread *blink1_thread =	
BareOSThreadCreateThread(blink1_mem,&blink1,0,500);

struct BareOSThread *blink2_thread =	
BareOSThreadCreateThread(blink2_mem,&blink2,0,500);

	struct BareOSThread *blink3_thread =	
		BareOSThreadCreateThread(blink3_mem,&blink3,0,500);

	struct BareOSThread *spi_thread =	
		BareOSThreadCreateThread(spi_memory,&spi,0,500);


//######END BAREOS INIT##########


BareOSSchedulerAddThread(spi_thread);

BareOSSchedulerAddThread(blink1_thread);
BareOSSchedulerAddThread(blink2_thread);

BareOSSchedulerAddThread(blink3_thread);

BareOSCallSwitch();	
//
//############################## END SYSTEM INIT @@@@@@@####################
//

uint8_t test[50] = {0b10101010};

DmaInit(&DMA2S1_OBJECT);

struct DmaConfig config = {.num_data = 50, .value_to_set = 0b10101111, 
	.to_address = data_out};
DmaSetMemory(&DMA2S1_OBJECT, &config);
config.value_to_set = 0b10001000;
DmaSetMemory(&DMA2S1_OBJECT, &config);

	while(1)
	{
		BareOSTimerDelayInterrupt(100);
	}

return 1;	
}

