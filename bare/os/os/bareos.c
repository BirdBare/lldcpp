//
//
//
//
//

#include "bareos.h"



#include "bareos_required_functions.h"

#include "timer_hal.h"

#include "bareos_timer.c"
#include "bareos_thread.c"
#include "bareos_scheduler.c"

extern uint8_t main_memory[];
void main(void *args);

extern uint8_t BAREOS_THREAD_NULL_MEMORY[];
void BAREOS_THREAD_NULL(void *args);

void BareOSEntry(void *estack)
{
BareOSDisableInterrupts();

SCB->CPACR |= 0b1111 << 20;
FPU->FPCCR |= 0b11 << 30;
//enable fpu, lazy stacking, etc.

asm volatile("DSB");
asm volatile("ISB");

NvicSetSystemInterruptPriority(PendSV_IRQn, 255);
NvicSetInterruptPriority(TIM6_DAC_IRQn, 100);
NvicEnableInterrupt(TIM6_DAC_IRQn);

BareOSThreadCreateThread(BAREOS_THREAD_NULL_MEMORY,&BAREOS_THREAD_NULL,0,1024);

BareOSSchedulerInitTick(100,0);
//init system

struct BareOSThread *main_thread =
BareOSThreadCreateThread(main_memory,&main,0,500);

BareOSSchedulerAddThread(main_thread);

BAREOS_SCHEDULER.current = estack;

BareOSCallSwitch();

BareOSEnableInterrupts();

BREAK(127);
//ENTRY point into

}



