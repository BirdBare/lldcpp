//
//
//
//
//



#ifndef schedule_H
#define schedule_H

#include "board.h"
#include "thread.h"

extern struct Schedule
{
	struct Thread *current_thread;
} SCHEDULER;

#define SCHEDULE_CURRENT_THREAD(void) SCHEDULER.current_thread






#endif
