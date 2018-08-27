//
//
//
//
//

#ifndef BAREOS_H
#define BAREOS_H

#include "board.h"
#include "mutex.h"

//
// OS FUNCTIONS
//
#define BareOSEnableInterrupts(void) \
do { \
	asm volatile("cpsie i"); \
} while(0) \

#define BareOSDisableInterrupts(void) \
do { \
	asm volatile("cpsid i"); \
}while(0) \





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

};

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

	union
	{
		uint16_t flags; //flags for the scheduler
		uint16_t pause:1; //prevent the scheduler from switching threads
		uint16_t:15;
	};

	struct 
	{
		struct BareOSTimer *list; //list of timers waiting on interrupt delay

		uint32_t milliseconds; //milliseconds the timer has been running
	} system_timer;

} extern BAREOS_SCHEDULER;

//
// FUNCTIONS
//
void BareOSSchedulerInit(uint32_t hz, uint32_t flags);
void BareOSSchedulerAddThread(struct BareOSThread *thread);
void BareOSSchedulerRemoveThread(struct BareOSThread *thread);
void BareOSCallSwitch(void);
static inline struct BareOSThread * BareOSSchedulerGetCurrentThread(void)
{
	return BAREOS_SCHEDULER.current;
}
static inline void BareOSSchedulerPause(void)
{
	BAREOS_SCHEDULER.pause = 1;
}

static inline void BareOSSchedulerResume(void)
{
	BAREOS_SCHEDULER.pause = 0;
}



//










//
// TIMER STRUCTURE
//
struct BareOSTimer
{
	struct BareOSTimer *next;
	//next timer in order from decreasing to increasing time remaining 

	uint32_t milliseconds; //milliseconds timer has been running

	void (*callback)(void *args);
	//function

	void *args;
	//args
};

//
//FUNCTIONS
//
uint32_t BareOSTimerGetTime(void);
void BareOSTimerDelayPolled(uint32_t milliseconds);
void BareOSTimerDelayInterrupt(uint32_t milliseconds);

//
// CALLBACKS
//
void BAREOS_SCHEDULER_TICK_CALLBACK(void *args);









#endif
