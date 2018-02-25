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
		waiter = {.waiting_thread = BareOSSchedulerGetCurrentThread()};
	//create the waiting mutex

	if(mutex->owner == 0 || mutex->owner == waiter.waiting_thread)
	//if nothing is in the mutex list
	{
		mutex->owner = waiter.waiting_thread;
		//make thread the owner and let it run

		mutex->counter++;
		//increment the times owner counter
	} else
	//if something owns the mutex
	{
		BareOSSchedulerRemoveThread(waiter.waiting_thread);
		//thread sleep function

		DllAddBefore(&mutex->list,&waiter.list);
		//add thread to the Prev position which is the last spot

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
		struct DllList *remove_list = mutex->list.next;
		//get mutex list next thing waiting

		if(remove_list != &mutex->list)
		//if next is not equal to the list head then another thread is waiting
		{
			mutex->owner = ((struct MutexWaiter *)remove_list)->waiting_thread;
			//set new owner

			DllRemove(remove_list);
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








