//
//
//
//
//


#include "timer_hal.h"


#include "bareos_scheduler.c"






uint32_t BareOsTimerGetTime(void)
{
	return BAREOS_TIMER_MASTER.milliseconds;
}






void BAREOS_SCHEDULER_TICK_CALLBACK(void *args)
{
	if(++BAREOS_TIMER_MASTER.milliseconds == BAREOS_SCHEDULER.milliseconds)
	{
		BareOSSchedulerSwitch();
		//call user defined interrupt that switches threads

		BAREOS_SCHEDULER.milliseconds += 1000 / BAREOS_SCHEDULER.hz;
		//set system timer for another interval everytime it goes off
	}
}










uint32_t BareOSTimerInit(struct TimerObject *timer_object, 
	struct TimerConfig *timer_config)
{
	if(timer_config->one_pulse != 0)
		return 1;
	
	TimerInit(timer_object);
	//do any timer initialization needed

	BAREOS_TIMER_MASTER.timer = timer_object;
	BAREOS_TIMER_MASTER.list = 0;
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
		BAREOS_TIMER_MASTER.timer->timer_config->clock_speed / 1000);
	//start a timer for 1ms if possible because we know a thread is running
}
















void BareOSTimerDelayPolled(uint32_t milliseconds)
{
	uint32_t milliseconds_ref = BAREOS_TIMER_MASTER.milliseconds;

	do
	{
		asm("");
		//non optimizable wait 
	} while((BAREOS_TIMER_MASTER.milliseconds - milliseconds_ref) < milliseconds);

	return;
}








