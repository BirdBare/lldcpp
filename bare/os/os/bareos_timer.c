//
//
//
//
//


#include "bareos.h"


static struct TimerConfig system_timer_config = {0};
#define BAREOS_SYSTEM_TIMER TIMER6_OBJECT



uint32_t BareOSTimerGetTime(void)
{
	return BAREOS_SCHEDULER.system_timer.milliseconds;
}

























void BareOSTimerDelayPolled(uint32_t milliseconds)
{
	uint32_t milliseconds_ref = BareOSTimerGetTime();

	do
	{
		BareOSCallSwitch();
		//non optimizable wait 
	} while((BareOSTimerGetTime() - milliseconds_ref) < milliseconds);

	return;
}


void BareOSTimerDelayInterrupt(uint32_t milliseconds)
{
	BareOSDisableInterrupts();

	struct BareOSTimer timer = {
		.milliseconds = milliseconds + BareOSTimerGetTime(),
		.callback = (void *)&BareOSSchedulerAddThread, 
		.args = BareOSSchedulerGetCurrentThread()};

	struct BareOSTimer *list_timer = (struct BareOSTimer *)&BAREOS_SCHEDULER.system_timer;

	while(list_timer->next != 0 && 
		timer.milliseconds > list_timer->next->milliseconds) 
	{
		list_timer = list_timer->next;
		//get next timer
	}
	//if timer is not zero and if timer is greater than next list timer then loop

	timer.next = list_timer->next;
	list_timer->next = &timer;
	//put timer in the list

	BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
	//remove thread from scheduler list. to put it to sleep

	BareOSEnableInterrupts();

	BareOSCallSwitch();
	//call scheduler to switch out
}






