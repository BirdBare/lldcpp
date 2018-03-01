//
//
//
//
//



#ifndef mutex_H
#define mutex_H

#include "bareos.h"

struct MutexWaiter
{
	struct MutexWaiter *next;
	struct MutexWaiter *prev;
	//list of threads waiting on the mutex
	
	void *waiting_thread; //thread waiting on mutex
};



struct Mutex
{
	struct MutexWaiter *list;
	//list of threads waiting on the mutex

	void *owner;
	//owner of the mutex

	uint32_t counter; //times owned counter (recursion counter)
};

static inline void MutexInit(struct Mutex *mutex)
{
	mutex->owner = 0;
	mutex->counter = 0;
}

void MutexLock(struct Mutex *mutex);

void MutexUnlock(struct Mutex *mutex);













#endif
