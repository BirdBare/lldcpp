//
//
//
//
//

#ifndef BAREOS_H
#define BAREOS_H

#include "board.h"
//
// THREAD STRUCTURE
//
struct BareOSThread
{
	struct BareOSThread *next;
	struct BareOSThread *prev;

	uint32_t stack_size; //size in bytes

	void *stack_pointer;

	uint32_t flags;
} BAREOS_THREAD_MAIN = {&BAREOS_THREAD_MAIN,&BAREOS_THREAD_MAIN,500},
		*BARE_THREAD_NULL;

//
// FUNCTIONS
//
struct BareOSThread * BareOSThreadCreateThread(void *thread_memory,
	void (*thread_function)(void *args), void *args, uint32_t stack_size);
//create thread memory static/dynamic











//
// SCHEDULER STRUCTURE
//
volatile struct
{
	struct BareOSThread *current; // 

	struct BareOSThread *list; //pointer to list of threads 
	//points to next thread or 0 if no threads 

	uint32_t milliseconds; //end milliseconds till next scheduling

	uint16_t hz; //frequency the scheduler is running

	uint16_t flags; //flags for the scheduler

} BAREOS_SCHEDULER;

//
// FUNCTIONS
//
void BareOSSchedulerInit(uint32_t hz, uint32_t flags);
void BareOSSchedulerAddThread(struct BareOSThread *thread);
void BareOSSchedulerRemoveThread(struct BareOSThread *thread);
static inline void BareOSSchedulerSwitch(void);
//










//
// TIMER STRUCTURE
//
struct BareOSTimer
{
	struct BareOSTimer *next;
	//next timer in order from decreasing to increasing time remaining 

	struct BareOSTimer *prev;
	//used to help move timers around 

	uint32_t milliseconds; //milliseconds timer has been running

	void (*callback)(void *args);
	//function

	void *args;
	//args
};

//
// TIMER MASTER STRUCTURE
//
volatile struct  
{ 
	struct TimerObject *timer; 
	struct BareOSTimer *list; 
	uint32_t milliseconds; 
} BAREOS_TIMER_MASTER = {0}; 

//
//FUNCTIONS
//
uint32_t BareOSTimerInit(struct TimerObject *timer_object,
	struct TimerConfig *timer_config);
uint32_t BareOSTimerStart(void);
uint32_t BareOsTimerGetTime(void);
void BareOSTimerDelayPolled(uint32_t milliseconds);
void BareOSTimerDelayInterrupt(uint32_t milliseconds);

//
// CALLBACKS
//
void BAREOS_SCHEDULER_TICK_CALLBACK(void *args);









#endif
