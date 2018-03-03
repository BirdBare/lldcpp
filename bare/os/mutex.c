//
//
//
//
//



#include "mutex.h"


void MutexLock(struct Mutex *mutex)
{	
	BareOSDisableInterrupts();
	//disable interrupts

	struct MutexWaiter 
		waiter = {
			.waiting_thread = BareOSSchedulerGetCurrentThread()};
	//create the waiting mutex

	if(mutex->owner == 0 || mutex->owner == waiter.waiting_thread)
	//if nothing is in the mutex list
	{
		mutex->owner = waiter.waiting_thread;
		//make thread the owner and let it run

		mutex->counter++;
		//increment the times owner counter
	} 
	//if something owns the mutex
	else
	{
			waiter.next = (void *)mutex;
			waiter.prev = mutex->prev;
			//make waiter part of list

			mutex->prev->next = &waiter;
			mutex->prev = &waiter;
			//add to the list

		BareOSSchedulerRemoveThread(waiter.waiting_thread);
		//thread sleep function

		BareOSCallSwitch();
	}

	BareOSEnableInterrupts();
	//enable interrupts and switch
}

void MutexUnlock(struct Mutex *mutex)
{
	BareOSDisableInterrupts();
	//disable interrupts

	if(--mutex->counter == 0)
	{
		struct MutexWaiter *remove_list = mutex->next;
		//get mutex list next thing waiting

		if((void *)mutex != (void *)remove_list)
		{
			mutex->owner = remove_list->waiting_thread;
			//set new owner

			struct MutexWaiter *next = remove_list->next;
			struct MutexWaiter *prev = remove_list->prev;

			next->prev = prev;
			prev->next = next;
			//remove the thread from the mutex list 

			mutex->counter = 1;
			//make counter 1 for new thread

			BareOSSchedulerAddThread(mutex->owner);
			//thread wakeup function
		}
		else
		//if no threads are left
		{
			mutex->owner = 0;
			//there is no owner

			mutex->counter = 0;
			//make counter zero for new mutex
		}
	}

	BareOSEnableInterrupts();
	//enable interrupts
}








