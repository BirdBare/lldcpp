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
#include "timer_lld.h"

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





struct BareThread
{
	struct BareThread *next;
	struct BareThread *prev;
	
	uint32_t stack_size; // size in bytes
	void *stack_pointer;

	uint32_t flags;

} *BARE_THREAD_MAIN, *BARE_THREAD_NULL;

void DEVICE_CREATE_REGISTER_STACK(void *e_stack, 
	void (*thread_function)(void *args),	void *args)
{
	asm volatile("mov r9, #0x1000000"); //set PSR Reset Value
	asm volatile("mov r8, r1"); //move functions address to program counter
	asm volatile("ldr r7, =BARE_THREAD_RETURN"); //link register is return func
	asm volatile("mov r6, #0"); //set r12 to zero. usually zero for new functions
	asm volatile("stmdb r0!, {r2-r9}"); //push essential registers to stack
	asm volatile("mov r10, #0xFFFFFFF9"); //exception return
	asm volatile("stmdb r0!, {r1-r10}"); //push dont care registers and exception
																			//return
}

void BareThreadCreateThread(void *thread_memory, 
	void (*thread_function)(void *args), void *args, uint32_t stack_size)
{
	struct BareThread *new_thread = thread_memory;
	//we are creating the new bare therad on the memory area

	new_thread->next = new_thread;
	new_thread->prev = new_thread;
	//must point to itself at first

	new_thread->flags = 0;
	//flag is set to zero and configured by user

	new_thread->stack_size = stack_size;
	new_thread->stack_pointer = thread_memory + stack_size;
	//set size and pointer start

	DEVICE_CREATE_REGISTER_STACK(new_thread->stack_pointer,
		thread_function, args);
	//call user created function that stacks thread registers for context switch
}



struct BareTimer
{
	struct BareTimer *next;
	struct BareTimer *prev;
	//next and prev
	
	uint32_t time_milliseconds;
	//time waiting

	void (*callback)(void *args);
	//function

	void *args;
	//args
};


struct BareTimerMaster
{
	struct TimerObject *timer;
		
	struct BareTimer *list; //points to timer being timed
	//list of timers
};


uint32_t BareTimerMasterInit(struct BareTimerMaster *master)
{
	if(master->timer == 0)
		return 1;

	

}


void BareTimerConfigTimer(struct BareTimer *timer); 

	








struct BareScheduler
{
	struct BareThread *current; // 

	struct BareThread *list; //pointer to list of threads 
													 //points to next thread or 0 if no threads	
	
	uint32_t tick_hz;

	uint32_t flags; //flags for the scheduler

} BARE_SCHEDULER = {0};

void BareSchedulerAddThread(struct BareThread *thread)
{
}

void BareSchedulerRemoveThread(struct BareThread *thread)
{
}

void DEVICE_CONTEXT_SWITCH(struct BareThread *switch_in,
	struct BareThread *switch_out)
{
	//start context save
	asm volatile("mrs r12, msp"); //get stack pointer for stacking rest of registers
	asm volatile("stmdb r12!, {r3-r11, lr}"); //store rest of registers and exec
																						//return. r3 added for alignment
	asm volatile("str r12, [r1, #8]"); //store stack pointer
	//end context save

	//start context load
	asm volatile("ldr r12, [r0, #8]"); //get new stack pointer to laod from
	asm volatile("ldmia r12!, {r3-r11, lr}"); //load rest of registers and exec
																							//return
	asm volatile("msr msp, r12"); //set new stack pointer
	//end context load
}

void BareSchedulerSchedule(void *args)
{
	if(BARE_SCHEDULER.list == 0)
	{
		DEVICE_CONTEXT_SWITCH(BARE_THREAD_NULL, 
			BARE_SCHEDULER.current);

		BARE_SCHEDULER.current = BARE_THREAD_NULL;
	}
	//check if no threads to schedule. 
	//runs null thread after setting a timer if threads are sleeping on a timer
	else
	{
		DEVICE_CONTEXT_SWITCH(BARE_SCHEDULER.current, 
			BARE_SCHEDULER.current->next);
		//user defined switch function
	}
	//set generic tick timer and switch to next thread

	return;
	//finished
}








void blink(void *args)
{
	uint8_t data[6] =
		{0b10000001,0b10000001,0b10000001,0b10000001,0b10000001,0b10000001};

	nokia.nokia_pins ^= 0b1 << LIGHT_BIT;

	SpiTransferInterrupt(&SPI1_OBJECT,data,data,1);
};

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

	LldTimerInit(&TIMER6_OBJECT);
	LldTimerInit(&TIMER7_OBJECT);

	struct TimerConfig timer6_config = {.milliseconds = 1000, .callback = &blink};
	LldTimerConfigTimerMilliseconds(&TIMER6_OBJECT, &timer6_config);
	LldTimerStartTimerInterrupt(&TIMER6_OBJECT);

	struct TimerConfig timer7_config = {.clock_speed = 10000};

	if(LldTimerConfigTimer(&TIMER7_OBJECT,&timer7_config) != 0)
		BREAK(40);

	while(1)
	{
		if(GpioGetInput(&GPIOA_OBJECT, PIN_0) != 0)
		{
			GpioToggleOutput(&GPIOD_OBJECT, PIN_13);

			if(LldTimerStartTimerPolledMilli(&TIMER7_OBJECT,1000) != 0)
				BREAK(50);
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

