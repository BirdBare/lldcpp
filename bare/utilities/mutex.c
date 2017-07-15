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
	} else
	//if something owns the mutex
	{
		DllAddPrev((struct DllNode *)&mutex->list, &thread_node);
		//add thread to the Prev position which is the last spot

		//thread sleep/wait function
	}

	mutex->counter++;
	//increment the times owner counter

	//enable interrupts
}

void MutexUnlock(struct Mutex *mutex)
{
	//disable interrupts

	DllRemove(mutex->list.next);
	//remove the thread from the mutex

	struct DllNode *thread_node = mutex->list.next;
	//get thread_node because we will use it more than once

	if(thread_node != 0)
	//if there is another thread
	{
		mutex->owner = thread_node->data;
		//make next thread the new owner


		//thread wakeup/continue function
	}
	else
	{
		mutex->owner = 0;
	}

	//make counter zero for new thread
	mutex->counter = 0;

	//enable interrupts
}








