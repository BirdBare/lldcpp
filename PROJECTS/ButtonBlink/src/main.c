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



void BARE_THREAD_RETURN(void)
{
BREAK(50);
}

struct BareThread
{
	struct BareThread *next;
	struct BareThread *prev;
	
	uint32_t stack_size; // size in bytes

	void *stack_pointer;

	uint32_t flags;

} BARE_THREAD_MAIN = {&BARE_THREAD_MAIN,&BARE_THREAD_MAIN,500}, *BARE_THREAD_NULL;

void * DEVICE_CREATE_REGISTER_STACK(void *e_stack, 
	void (*thread_function)(void *args),	void *args);

struct BareThread * BareThreadCreateThread(void *thread_memory, 
	void (*thread_function)(void *args), void *args, uint32_t stack_size)
{
	struct BareThread *new_thread = thread_memory;
	//we are creating the new bare therad on the memory area

	new_thread->next = new_thread;
	new_thread->prev = new_thread;
	//must point to itself at first

	new_thread->flags = 0;
	//flag is set to zero and configured by user

	stack_size -= sizeof(struct BareThread);

	new_thread->stack_size = stack_size;
	new_thread->stack_pointer = thread_memory + stack_size;
	//set size and pointer start

	new_thread->stack_pointer = DEVICE_CREATE_REGISTER_STACK(
		new_thread->stack_pointer, thread_function, args);
	//call user created function that stacks thread registers for context switch

	return new_thread;
}








volatile struct 
{
	struct BareThread *current; // 

	struct BareThread *list; //pointer to list of threads 
													 //points to next thread or 0 if no threads	
	
	uint32_t milliseconds; //end milliseconds till next scheduling

	uint32_t tick_hz; //frequency the scheduler is running

	uint32_t flags; //flags for the scheduler

} BARE_SCHEDULER = {&BARE_THREAD_MAIN,&BARE_THREAD_MAIN,10};

void BareSchedulerAddThread(struct BareThread *thread)
{
}

void BareSchedulerRemoveThread(struct BareThread *thread)
{
}

void DEVICE_CONTEXT_SWITCH(struct BareThread *switch_in,
	struct BareThread *switch_out);

void BareSchedulerSchedule(void *args)
{
	asm volatile("ldr r0, =BARE_SCHEDULER");
	asm volatile("ldr r1, [r0, #0]");
	asm volatile("ldr r0, [r0, #4]");
	asm volatile("mrs r12, psp");
	asm volatile("stmdb r12!, {r3-r11, lr}");
	asm volatile("str r12, [r1, #12]");

	asm volatile("ldr r12, [r0, #12]");
	asm volatile("ldmia r12!, {r3-r11, lr}");
	asm volatile("msr psp, r12");
	asm volatile("");


		BARE_SCHEDULER.current = BARE_SCHEDULER.list;
		BARE_SCHEDULER.list = BARE_SCHEDULER.list->next;

	return;
	//finished
}










struct SystemTimer
{
	struct SystemTimer *next;
	//next timer in order from decreasing to increasing time remaining 
		
	struct SystemTimer *prev;
	//used to help move timers around	

	uint32_t milliseconds; //milliseconds timer has been running

	void (*callback)(void *args);
	//function

	void *args;
	//args
};

volatile struct 
{
	struct TimerObject *timer;
	struct SystemTimer *list;
	uint32_t milliseconds;
} SYSTEM_TIMER_BASE = {0};


void SYSTEM_TIMER_TICK_CALLBACK(void *args)
{
	SYSTEM_TIMER_BASE.milliseconds++;

		BareSchedulerSchedule(0);
}


uint32_t SystemTimerInit(struct TimerObject *timer_object, 
	struct TimerConfig *timer_config)
{
	if(timer_config->one_pulse != 0)
		return 1;
	
	TimerInit(timer_object);
	//do any timer initialization needed

	SYSTEM_TIMER_BASE.timer = timer_object;
	SYSTEM_TIMER_BASE.list = 0;
	SYSTEM_TIMER_BASE.milliseconds = 0;
	//setup the system timer base
	
	timer_config->callback = &SYSTEM_TIMER_TICK_CALLBACK;
	//set callback function for ticking timer

	if(TimerConfigTimer(timer_object,timer_config) != 0)
		return 1;
	//config the timer

	return 0;
}

uint32_t SystemTimerStart(void)
{
	return TimerStartTimerInterrupt(SYSTEM_TIMER_BASE.timer, 10);
	//start a timer for 1ms if possible because we know a thread is running
}

void SystemTimerDelayPolled(uint32_t milliseconds)
{
	uint32_t milliseconds_ref = SYSTEM_TIMER_BASE.milliseconds;

	do
	{
		asm("");
		//non optimizable wait 
	} while((SYSTEM_TIMER_BASE.milliseconds - milliseconds_ref) < milliseconds);

	return;
}















uint8_t blink_memory[500]; 

void blink(void *args)
{
	uint8_t data[6] =
		{0b10000001,0b10000001,0b10000001,0b10000001,0b10000001,0b10000001};

	while(1)
	{
		nokia.nokia_pins ^= 0b1 << LIGHT_BIT;

			SpiTransferInterrupt(&SPI1_OBJECT,data,data,1);

			SystemTimerDelayPolled(1000);
	}
};

uint8_t main_space[500];

int main(void)
{
	asm volatile("mrs r2, MSP");
	asm volatile("sub r2, #500");
	asm volatile("msr PSP, r2");
	asm volatile("mrs r1, CONTROL");
	asm volatile("mov r2, #0b10");
	asm volatile("orr r1, r2");
	asm volatile("msr CONTROL, r1");
	asm volatile("ISB");
	
	FlashEnableArt(&FLASH_OBJECT);
	struct FlashConfig flash_config = {5};
	FlashConfig(&FLASH_OBJECT,&flash_config);
	//Enable Art Controller and set wait states

	struct ClockConfig clock_config = {168000000,168000000,42000000,84000000};
	ClockConfig(&clock_config);
	//configure the cpu clocks

	struct BareThread *blink_thread =	
		BareThreadCreateThread(blink_memory,&blink,0,500);

		blink_thread->next = &BARE_THREAD_MAIN;
		BARE_THREAD_MAIN.next = blink_thread;



	struct TimerConfig system_config = {.clock_speed = 10000};
	SystemTimerInit(&TIMER6_OBJECT,&system_config);
	SystemTimerStart();
	//initalize system timer

	
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
			SystemTimerDelayPolled(1000);

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

