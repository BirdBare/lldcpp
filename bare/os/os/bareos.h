//
//
//
//
//

#ifndef BAREOS_H
#define BAREOS_H

#include "board.h"
#include "dll.h"

//
// OS FUNCTIONS
//
static inline void BareOSEnableInterrupts(void)
{
	asm volatile("cpsie i");
}

static inline void BareOSDisableInterrupts(void)
{
	asm volatile("cpsid i");
}





//
// THREAD STRUCTURE
//
struct BareOSThread
{
	void *stack_pointer;

	struct BareOSThread *next;
	struct BareOSThread *prev;

	uint32_t stack_size; //size in bytes

	uint32_t flags;

} BAREOS_THREAD_MAIN;

//
// FUNCTIONS
//
struct BareOSThread * BareOSThreadCreateThread(void *thread_memory,
	void (*thread_function)(void *args), void *args, uint32_t stack_size);
//create thread memory static/dynamic











//
// SCHEDULER STRUCTURE
//
volatile struct BareOSScheduler
{
	struct BareOSThread *current; // 

	struct BareOSThread *list; //pointer to list of threads 
	//points to next thread or 0 if no threads 

	uint32_t milliseconds; //end milliseconds till next scheduling

	uint16_t hz; //frequency the scheduler is running

	uint16_t flags; //flags for the scheduler

} extern BAREOS_SCHEDULER;

//
// FUNCTIONS
//
void BareOSSchedulerInit(uint32_t hz, uint32_t flags);
void BareOSSchedulerAddThread(struct BareOSThread *thread);
void BareOSSchedulerRemoveThread(struct BareOSThread *thread);
void BareOSCallSwitch(void);
static inline void BareOSSchedulerSwitch(void);
static inline struct BareOSThread * BareOSSchedulerGetCurrentThread(void)
{
	return BAREOS_SCHEDULER.current;
}
//










//
// TIMER STRUCTURE
//
struct BareOSTimer
{
	struct DllList list;
	//next timer in order from decreasing to increasing time remaining 

	uint32_t milliseconds; //milliseconds timer has been running

	void (*callback)(void *args);
	//function

	void *args;
	//args
};

//
// TIMER MASTER STRUCTURE
//
volatile struct BareOSTimerMaster  
{ 
	struct DllList list; //list of timers waiting on interrupt delay

	struct TimerObject *timer; //timer object the master timer controls to time

	uint32_t milliseconds; //milliseconds the timer has been running

} extern BAREOS_TIMER_MASTER; 

#include "timer_hal.h"

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
