//
//
//
//
//



#ifndef thread_H
#define thread_H

#include "board.h"
#include "dll.h"

extern struct Thread
{
	struct DllList list; //active threads list in the scheduler

	void *stack_pointer_start; //absolute start of the stack
	void *stack_pointer_current; //current pointer address at the stack

	union
	{
		uint32_t flags; //flags whole variable
		struct //flags inside the flag variable.
		{
			union
			{
				uint32_t event_flags:4;	//flag for how many events we are waiting on.

				struct
				{
					//LSB
					uint32_t event_number:3; //number of events the thread is waiting on
					uint32_t event_and_or_flag:1; //if listener is waiting for all flags
																				//or one flag out of many.
#define EVENT_AND_FLAG 0
#define EVENT_OR_FLAG 1
					//MSB
				};
			};

			uint32_t :28;
		};
	};
} MAIN;








#endif
