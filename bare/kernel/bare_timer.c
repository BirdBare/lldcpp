//
//
//
//
//

#include "bare_timer.h"
#include "bare_system.h"

volatile uint32_t timer_milliseconds = 0;

volatile struct TimerObject timer_list_start = 
	{&timer_list_start, &timer_list_start, 0};

#ifdef BAREOS
void TimerCreateTimer(
	uint32_t timeout_time,
	void (*timeout_function)(void *parameter),
	void *timeout_parameter)
{
	volatile struct TimerObject *timer_finder = &timer_list_start;
	//get the start of the timer list

	SystemDisableInterrupts();
	//Disable Interrupts here

	uint32_t current_time = TimerGetMilli();
	//get current time because we will use it a lot

	do
	{
		timer_finder = timer_finder->prev;
		//traverse the list starting at the longest timer

	}while((timer_finder->timeout_time - current_time) > timeout_time);
	//keep going until we find a timer with a smaller timeout time than our new
	//timer

	//add timer after the timer we found
	struct TimerObject timer = {timer_finder->next,timer_finder, 
		timeout_time + current_time, timeout_function, timeout_parameter};

	timer_finder->next->prev = &timer;
	timer_finder->next = &timer;
	//finish adding timer to list


	SystemEnableInterrupts();
	//enable Interrupts here

	//put thread to sleep

	return;

}
#endif

void TimerDelayMilli(const uint32_t milli)
{
	const uint32_t milli_ref = TimerGetMilli();

	do
	{
		WAIT;
	} while((TimerGetMilli() - milli_ref) < milli);

	return;
}


void TIMER_ISR(void)
{
	TimerTickTimer();
}


