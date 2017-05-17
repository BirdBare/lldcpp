//
//
//
//
//



#ifndef TIMER_H
#define TIMER_H

#include "board.h"
#include "bare_defines.h"

//******************************************************************************
//	
//									TIMER Structures	 
//	
//******************************************************************************

struct TimerObject
{
	volatile struct TimerObject *next;	
	volatile struct TimerObject *prev;	
	//Dually linked list so adds/removes are quicker
	
	uint32_t timeout_time; //timeout_time + timer_milliseconds

	void (*timeout_function)(void *parameter);
	//timeout function callback. Called when the timer reaches it's timeout time

	void *timeout_parameter;
	//parameter for the timeout function. If more than one parameter it must
	//be an array of parameters
};


//******************************************************************************
//	
//									TIMER Variables and Get Variable definitions	 
//	
//******************************************************************************


extern volatile uint32_t timer_milliseconds;
//holds current timer time


extern volatile struct TimerObject timer_list_start;
//Start of the timer list. Not used. Next is the timer with the shortest 
//timeout time and increases timeout_time as you traverse. prev is the timer
//with the longest timeout time and decreases timeout_time as you traverse.
//timeout_time always equals the current timer + 1 to make it the shortest timer


//******************************************************************************
//	
//									TIMER USER DEFINED FUNCTIONS	 
//	
//******************************************************************************

#ifndef TIMER_INIT_FUNCTION
#error "Must define an initialization function for timer as TIMER_INIT_FUNCTION"
#endif
#define TimerInit(void) TIMER_INIT_FUNCTION()

#ifndef TIMER_UPDATE_FUNCTION
#error "Must define a way to update timer tick rate as TIMER_UPDATE_FUNCTION"
#endif
#define TimerUpdateTickRate(void) TIMER_UPDATE_FUNCTION()

#ifdef BAREOS
#ifndef TIMER_ISR
#error "Must Define TIMER_ISR as the millisecond timer isr vector"
#endif
void TIMER_ISR(void);
#endif

//******************************************************************************
//	
//									TIMER ISR Functions	 
//	
//******************************************************************************

#define TimerGetMilli(void) timer_milliseconds
//returns the current timer time


// This function is only called inside the TIMER_ISR function
ALWAYS_INLINE void TimerTickTimer(void)
{
	uint32_t current_time = ++timer_milliseconds;
	//increment the timer milliseconds then place into variables

	timer_list_start.timeout_time = current_time + 1;
	//make timeout time for list start always 1 before the current time so it 
	//skips the while loop and to make adding easier in TimerCreateTimer

	volatile struct TimerObject *shortest_timer = timer_list_start.next;
	//get the timer with the shortest timeout time

	uint32_t timeout_time;
	//timeout_time variable for loop checking

	while((timeout_time = shortest_timer->timeout_time) == current_time)
	//if timer_milliseconds has reached the timers timeout time
	//loop for multiple timers with same timeout time
	{
		volatile struct TimerObject *next = shortest_timer->next;
		volatile struct TimerObject *prev = shortest_timer->prev;
		//put next and prev into variables to speed up compiler

		next->prev = prev;
		prev->next = next;
		//remove the timer

		shortest_timer->timeout_function(shortest_timer->timeout_parameter);
		//call the callback function for that timer with parameters.

		shortest_timer = shortest_timer->next;
		//get next timer incase it has same timeout time.
	}

	return;
	//finished so we return
}

//******************************************************************************
//	
//									USER TIMER FUNCTIONS	 
//	
//******************************************************************************
//User Called Function to create a timer
void TimerCreateTimer(
	uint32_t timeout_time,
	void (*timeout_function)(void *parameter),
	void *timeout_parameter);

#ifndef BAREOS
void TimerDelayMilli(const uint32_t milli);
#else
#define TimerDelayMilli( milli ) TimerCreateTimer(milli,
#endif


#endif
