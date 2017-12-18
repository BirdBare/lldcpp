//
//
//
//
//

#include "bareos_thread.c"

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

}









void BareOSSchedulerRemoveThread(struct BareOSThread *thread)
{

	struct BareOSThread *next = thread->next, *prev = thread->prev;

	if(BAREOS_SCHEDULER.list == BAREOS_SCHEDULER.list->next)
	{
		BAREOS_SCHEDULER.list = 0; //if only one thread left then list can equal 0
	}
	else
	{
		next->prev = prev; //remove thread from list
		prev->next = next;
	thread->next = thread;
	thread->prev = thread; //restore thread if we need to add it again
	}

}









static inline void BareOSSchedulerSwitch(void)
{
	SCB->ICSR |= 1 << 28;
}



