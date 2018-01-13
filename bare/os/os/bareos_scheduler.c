//
//
//
//
//

#include "bareos.h"

volatile struct BareOSScheduler BAREOS_SCHEDULER;

void BareOSSchedulerInit(uint32_t hz, uint32_t flags)
{
	BAREOS_SCHEDULER.current = &BAREOS_THREAD_MAIN;	
	BAREOS_SCHEDULER.list= &BAREOS_THREAD_MAIN;	
	BAREOS_SCHEDULER.milliseconds = BareOsTimerGetTime() + (1000 / hz);
	BAREOS_SCHEDULER.hz = hz;
	BAREOS_SCHEDULER.flags = flags;
}






void BareOSSchedulerAddThread(struct BareOSThread *thread)
{
	BareOSDisableInterrupts();

	struct BareOSThread *list_thread = BAREOS_SCHEDULER.list;

	if(list_thread == 0)
	{
		BAREOS_SCHEDULER.list = thread; //if list is zero we can just make list
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

	BareOSEnableInterrupts();
}









void BareOSSchedulerRemoveThread(struct BareOSThread *thread)
{
	BareOSDisableInterrupts();

	struct BareOSThread *next = thread->next, *prev = thread->prev;

	if(BAREOS_SCHEDULER.list == BAREOS_SCHEDULER.list->next)
	{
		BAREOS_SCHEDULER.list = 0; //if only one thread left then list can equal 0
	}
	else
	{
		next->prev = prev; //remove thread from list
		prev->next = next;
	}
	BareOSEnableInterrupts();
}
















int8_t BAREOS_THREAD_NULL_MEMORY[256];

void BAREOS_THREAD_NULL(void *args)
{
	while(1)
	{
	}
}






static inline void BareOSSchedulerSwitch(void)
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
asm volatile("bkpt 100 /*no threads in list*/");

asm volatile("SKIP_NULL_THREAD:"); //label used for skipping if not zero

//load next thread
DEVICE_LOAD_REGISTERS_STACK();

//make loaded thread the new current thread
asm volatile("str r0, [r12]"); //store list pointer into current pointer

//move to next thread
asm volatile("ldr r0, [r0, #4]"); //get new list pointer. new next thread
asm volatile("str r0, [r12, #4]"); //store new list pointer
}












void BAREOS_SCHEDULER_TICK_CALLBACK(void *args)
{
	uint32_t milliseconds = BAREOS_TIMER_MASTER.milliseconds++;

	struct BareOSTimer *timer = BAREOS_TIMER_MASTER.list;

	while(timer != 0 && milliseconds == timer->milliseconds)
	{
		timer->callback(timer->args);
		//call end timer function

		BAREOS_TIMER_MASTER.list = timer = timer->next;
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









