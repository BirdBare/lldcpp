//
//
//
//
//



#include "event.h"

void EventSignalFlags(struct EventSource *source, uint32_t signal_flags)
{
	struct DllList *listener_list = &source->listeners;

	struct EventListener *listener;

	

	while((listener = (struct EventListener *)listener_list->next) != 0)
	{
		uint32_t flags = listener->flags;

		signal_flags = flags & ~signal_flags;
		//try to modify flags

		if(signal_flags != flags)
		//if flags was modified
		{
			struct Thread *thread = listener->listener;
			//get thread listening

			if( signal_flags == 0 || thread->event_and_or_flag == EVENT_OR_FLAG)
			//if all flags are off or if any flag can be off then try to fire
			{
				DllRemove((struct DllNode *)&listener->list);
				//if the thread is signaled then it is removed

				if(--thread->event_number == 0)
				//if the event number is zero then we fire away
				{
					//wake thread
				}
			}
		}
	}
}










