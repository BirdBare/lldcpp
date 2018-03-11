//
//
//
//
//

#include "bareos.h"

volatile struct BareOSScheduler BAREOS_SCHEDULER;

uint8_t BAREOS_THREAD_NULL_MEMORY[1024];
void BAREOS_THREAD_NULL(void *args)
{
	while(1)
	{

		//go to sleep until next interrupt

		BareOSCallSwitch();
		//call switch
	}
}






void BareOSSchedulerInitTick(uint32_t hz, uint32_t flags)
{
	BAREOS_SCHEDULER.system_timer.list = 0;
	BAREOS_SCHEDULER.system_timer.milliseconds = 0;

	system_timer_config.callback = &BAREOS_SCHEDULER_TICK_CALLBACK;
	system_timer_config.tick_frequency = 10000;
	//config timer first

	BAREOS_SCHEDULER.current = 0;	
	BAREOS_SCHEDULER.list= (struct BareOSThread *)BAREOS_THREAD_NULL_MEMORY;	
	BAREOS_SCHEDULER.milliseconds = BareOSTimerGetTime() + (1000 / hz);
	BAREOS_SCHEDULER.hz = hz;
	BAREOS_SCHEDULER.flags = flags;
	//config scheduler

	TimerInit(&BAREOS_SYSTEM_TIMER);
	TimerConfigTimer(&BAREOS_SYSTEM_TIMER, &system_timer_config);
	TimerStartTimerInterrupt(&BAREOS_SYSTEM_TIMER, 
		BAREOS_SYSTEM_TIMER.timer_config->tick_frequency / 1000);
}


uint8_t BAREOS_THREAD_NULL_MEMORY[];



void BareOSSchedulerAddThread(struct BareOSThread *thread)
{
	struct BareOSThread *list_thread = BAREOS_SCHEDULER.list;

	if(list_thread == (struct BareOSThread *)BAREOS_THREAD_NULL_MEMORY)
	{
		BAREOS_SCHEDULER.list = thread; //if list is zero we can just make list
		thread->next = thread;
		thread->prev = thread;
	}
	else
	{
		thread->prev = list_thread;
		thread->next = list_thread->next;
		//make new thread point to next and prev
	
		list_thread->next->prev = thread; //make next thread prev point to new thread
		list_thread->next = thread; //make list next point to thread
	}
	//if threads are in list then we have to add to list of threads
}









void BareOSSchedulerRemoveThread(struct BareOSThread *thread)
{
	struct BareOSThread *next = thread->next, *prev = thread->prev;

	if(BAREOS_SCHEDULER.list == BAREOS_SCHEDULER.list->next)
	{
		BAREOS_SCHEDULER.list = (struct BareOSThread *)BAREOS_THREAD_NULL_MEMORY; 
		//if only one thread left then list can equal 0
	}
	else
	{
		next->prev = prev; //remove thread from list
		prev->next = next;

		if(thread == BAREOS_SCHEDULER.list)
		{
			BAREOS_SCHEDULER.list = next;
		}
		//if next thread then we have to change list pointer to complete removal
		
	}
}


















#define BareOSSchedulerSwitch(void) \
do { \
BareOSDisableInterrupts(); \
asm volatile("ldr r12, =BAREOS_SCHEDULER"); \
asm volatile("ldr r0, [r12, #0]"); \
DEVICE_SAVE_REGISTERS_STACK();\
asm volatile("ldr r0, [r12, #4]"); \
asm volatile("ldr r1, [r0, #4]"); \
asm volatile("str r1, [r12, #4]"); \
DEVICE_LOAD_REGISTERS_STACK(); \
asm volatile("str r0, [r12]"); \
BareOSEnableInterrupts(); \
asm volatile("bx lr"); \
} while(0);
/*static inline void BareOSSchedulerSwitch(void)
{

//get current thread
asm volatile("ldr r12, =BAREOS_SCHEDULER"); //get bareos scheduler pointer
asm volatile("ldr r0, [r12, #0]"); //get current thread


//save current thread
DEVICE_SAVE_REGISTERS_STACK();

//get next thread
asm volatile("ldr r0, [r12, #4]"); //load list pointer which is next thread

//if list pointer is zero then we need to run the null thread
asm volatile("cbnz r0, SKIP_NULL_THREAD"); //if not zero then more threads

//load null thread pointer for load
asm volatile("bkpt 100");

asm volatile("SKIP_NULL_THREAD:"); //label used for skipping if not zero

//load next thread
DEVICE_LOAD_REGISTERS_STACK();

//make loaded thread the new current thread
asm volatile("str r0, [r12]"); //store list pointer into current pointer

//move to next thread
asm volatile("ldr r0, [r0, #4]"); //get new list pointer. new next thread
asm volatile("str r0, [r12, #4]"); //store new list pointer
}*/

__attribute__((naked))
void PendSV_Handler(void)
{
	BareOSSchedulerSwitch();
}









void BAREOS_SCHEDULER_TICK_CALLBACK(void *args)
{
	uint32_t milliseconds = BAREOS_SCHEDULER.system_timer.milliseconds++;

	struct BareOSTimer *timer = BAREOS_SCHEDULER.system_timer.list;

	while(&timer->next != 0 && milliseconds == timer->milliseconds)
	{
		timer->callback(timer->args);
		//call end timer function

		BAREOS_SCHEDULER.system_timer.list = timer = timer->next;
		//get rid of timer and set next timer as list
	}

	if(milliseconds == BAREOS_SCHEDULER.milliseconds)
	{
		BAREOS_SCHEDULER.milliseconds += 1000 / BAREOS_SCHEDULER.hz;
		//set system timer for another interval everytime it goes off

		BareOSCallSwitch();
		//call Scheduler Switch function
	}
}









