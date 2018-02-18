//
//
//
//
//



#include "mutex.h"


void MutexLock(struct Mutex *mutex)
{	
	struct MutexWaiter 
		waiter = {.waiting_thread = BareOSSchedulerGetCurrentThread()};
	//create the waiting mutex

	BareOSDisableInterrupts();
	//disable interrupts

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
	struct DllList *remove_list = &mutex->list;
	//get mutex list

	BareOSDisableInterrupts();
	//disable interrupts

	if(DllGetNext(remove_list) != &mutex->list)
	//if there is another thread by seeing if next is same
	{
		remove_list = DllGetNext(remove_list);
		//get actual thread that is waiting

		mutex->owner = 
			((struct MutexWaiter *)DllGetNext(remove_list))->waiting_thread;
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

	BareOSEnableInterrupts();
	//enable interrupts
}








