//
//
//
//
//


#include "bareos.h"



volatile struct BareOSTimerMaster BAREOS_TIMER_MASTER = {0};


uint32_t BareOSTimerGetTime(void)
{
	return BAREOS_TIMER_MASTER.milliseconds;
}








uint32_t BareOSTimerInit(struct TimerObject *timer_object, 
	struct TimerConfig *timer_config)
{
	if(timer_config->one_pulse != 0)
		return 1;
	
	TimerInit(timer_object);
	//do any timer initialization needed

	BAREOS_TIMER_MASTER.timer = timer_object;
	BAREOS_TIMER_MASTER.list.next = (struct DllList *)&BAREOS_TIMER_MASTER.list;
	BAREOS_TIMER_MASTER.list.prev = (struct DllList *)&BAREOS_TIMER_MASTER.list;
	BAREOS_TIMER_MASTER.milliseconds = 0;
	//setup the system timer base
	
	timer_config->callback = &BAREOS_SCHEDULER_TICK_CALLBACK;
	//set callback function for ticking timer

	if(TimerConfigTimer(timer_object,timer_config) != 0)
		return 1;
	//config the timer

	return 0;
}











uint32_t BareOSTimerStart(void)
{
	return TimerStartTimerInterrupt(BAREOS_TIMER_MASTER.timer, 
		BAREOS_TIMER_MASTER.timer->timer_config->tick_frequency / 1000);
	//start a timer for 1ms if possible because we know a thread is running
}
















void BareOSTimerDelayPolled(uint32_t milliseconds)
{
	uint32_t milliseconds_ref = BareOSTimerGetTime();

	do
	{
		asm volatile ("");
		//non optimizable wait 
	} while((BareOSTimerGetTime() - milliseconds_ref) < milliseconds);

	return;
}


void BareOSTimerDelayInterrupt(uint32_t milliseconds)
{
	BareOSDisableInterrupts();

	struct BareOSTimer timer = {
		.milliseconds = milliseconds + BAREOS_TIMER_MASTER.milliseconds,
		.callback = (void *)&BareOSSchedulerAddThread, 
		.args = BareOSSchedulerGetCurrentThread()};

	struct BareOSTimer *list_timer = (struct BareOSTimer *)&BAREOS_TIMER_MASTER;

	while(DllGetNext(&list_timer->list) != &BAREOS_TIMER_MASTER.list && 
		((struct BareOSTimer *)DllGetNext(&list_timer->list))->milliseconds < 
			timer.milliseconds)
	{
		list_timer = DllGetNext(&list_timer->list);
		//get next timer
	}
	//if timer is not zero and if timer is greater than next list timer then loop

	DllAddAfter(&list_timer->list,&timer.list);
	//put timer in the list

	BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
	//remove thread from scheduler list. to put it to sleep

	BareOSEnableInterrupts();

	BareOSCallSwitch();
	//call scheduler to switch out
}






