//
//
//
//
//



#ifndef events_H
#define events_H

#include <stdint.h>
#include "dll.h"
#include "board.h"

//event source can be a single source.
struct EventSource
{
	struct DllList listeners; //list of listeners on the event
};

//event listener can be an array of event_handler functions or a thread
struct EventListener
{
	struct DllList list; //list of other listeners on the event source
	
	struct Thread *listener; //listening thread on this event

	uint32_t flags; //flags the event is listening/waiting for.
									//High bits are flags being waited for
};

#define EventCreateListener(name, flags) \
	struct EventListener name = {0,0,SCHEDULE_CURRENT_THREAD,flags}

ALWAYS_INLINE void EventWaitOrSingle(
	struct EventListener *listener,
	struct EventSource *source)
{
	SCHEDULE_CURRENT_THREAD()->event_flags = 0 | EVENT_OR_FLAG << 3 ;

	DllAddNextLinear(&source->listeners,&listener->list);
	//add listener to source list
}

ALWAYS_INLINE void EventWaitAndSingle(
	struct EventListener *listener,
	struct EventSource *source)
{
	SCHEDULE_CURRENT_THREAD()->event_flags = 0 | EVENT_AND_FLAG << 3 ;

	DllAddNextLinear(&source->listeners,&listener->list);
	//add listener to source list
}

ALWAYS_INLINE void EventWaitOrMultiple(
	struct EventListener *listener[],
	struct EventSource *source[],
	uint32_t num_source)
{
	SCHEDULE_CURRENT_THREAD()->event_flags = num_source | EVENT_OR_FLAG << 3 ;

	do
	{
		num_source--;
		DllAddNextLinear(&source[num_source]->listeners,&listener[num_source]->list);
	}while(num_source != 0);
	//add listeners to source lists
}

ALWAYS_INLINE void EventWaitAndMultiple(
	struct EventListener *listener[],
	struct EventSource *source[],
	uint32_t num_source)
{
	SCHEDULE_CURRENT_THREAD()->event_flags = num_source | EVENT_AND_FLAG << 3 ;

	do
	{
		num_source--;
		DllAddNextLinear(&source[num_source]->listeners,&listener[num_source]->list);
	}while(num_source != 0);
	//add listeners to source lists
}



void EventSignalFlags(struct EventSource *source, uint32_t flags);




#endif
