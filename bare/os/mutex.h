//
//
//
//
//



#ifndef mutex_H
#define mutex_H

#include <stdint.h>
#include "dll.h"

struct Mutex
{
	struct DllList list;
	//list of threads waiting on the mutex

	void *owner;
	//owner of the mutex

	uint32_t counter; //times owned counter (recursion counter)
};

void MutexLock(struct Mutex *mutex, void *thread);

void MutexUnlock(struct Mutex *mutex);













#endif
