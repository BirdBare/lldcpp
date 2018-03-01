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
			.waiting_thread = BareOSSchedulerGetCurrentThread(),
			.next = &waiter,
			.prev = &waiter};
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
		if(mutex->list == 0)
		//if nothing else is in the thread
		{
			mutex->list = &waiter;
			//add to list
		}
		//if list has mutex already waiting
		else
		{
			struct MutexWaiter *list_waiter = mutex->list;
			//make variable for waiter in the list

			waiter.next = list_waiter;
			waiter.prev = list_waiter->prev;
			//make waiter part of list

			list_waiter->prev->next = &waiter;
			list_waiter->prev = &waiter;
			//add to the list
		}

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
		struct MutexWaiter *remove_list = mutex->list;
		//get mutex list next thing waiting

		if(remove_list != 0)
		//if next is not equal to the list head then another thread is waiting
		{
			mutex->owner = remove_list->waiting_thread;
			//set new owner

			struct MutexWaiter *next = mutex->list->next;
			struct MutexWaiter *prev = mutex->list->prev;

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








