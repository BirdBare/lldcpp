//
//
//
//
//



#include "mutex.h"


void MutexLock(struct Mutex *mutex, void *thread)
{
	DllCreateStaticLinear(thread_node, thread);
	//create a spot for the list if needed

	//disable interrupts

	if(mutex->owner == 0 || mutex->owner == thread)
	//if nothing is in the mutex list
	{
		mutex->owner = thread;
		//make thread the owner and let it run

		mutex->counter++;
		//increment the times owner counter
	} else
	//if something owns the mutex
	{
		DllAddPrev((struct DllNode *)&mutex->list, &thread_node);
		//add thread to the Prev position which is the last spot

		//thread sleep/wait function
	}


	//enable interrupts
}

void MutexUnlock(struct Mutex *mutex)
{
	//disable interrupts


	struct DllNode *thread_node = mutex->list.next;
	//get thread_node because we will use it more than once

	if(thread_node != 0)
	//if there is another thread
	{
		mutex->owner = DllRemove(thread_node);
		//remove the next thread from the mutex list and make thread the new owner

		mutex->counter = 1;
		//make counter 1 for new thread
		
		//thread wakeup/continue function
	}
	else
	//if no threads are left
	{
		mutex->owner = 0;
		//there is no owner

		mutex->counter = 0;
		//make counter zero for new mutex
	}


	//enable interrupts
}








