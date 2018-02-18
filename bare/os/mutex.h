//
//
//
//
//



#ifndef mutex_H
#define mutex_H

#include "bareos.h"
#include "dll.h"
struct Mutex
{
	struct DllList list;
	//list of threads waiting on the mutex

	void *owner;
	//owner of the mutex

	uint32_t counter; //times owned counter (recursion counter)
};

struct MutexWaiter
{
	struct DllList list;
	//list of threads waiting on the mutex
	
	void *waiting_thread; //thread waiting on mutex
};

static inline void MutexInit(struct Mutex *mutex)
{
	mutex->list.next = &mutex->list;
	mutex->list.prev = &mutex->list;
	mutex->owner = 0;
	mutex->counter = 0;
}

void MutexLock(struct Mutex *mutex);

void MutexUnlock(struct Mutex *mutex);













#endif
