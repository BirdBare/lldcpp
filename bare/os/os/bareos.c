//
//
//
//
//

#include "bareos.h"



#include "bareos_required_functions.h"



#include "bareos_timer.c"
#include "bareos_thread.c"
#include "bareos_scheduler.c"

extern uint8_t main_memory[];
void main(void *args);

void BareOSEntry(void *estack)
{

SCB->CPACR |= 0b1111 << 20;
FPU->FPCCR |= 0b11 << 30;
//enable fpu, lazy stacking, etc.

asm volatile("DSB");
asm volatile("ISB");

BareOSSchedulerInit(1000,0);
//init system

struct BareOSThread *main_thread =
BareOSThreadCreateThread(main_memory,&main,0,500);

BareOSSchedulerAddThread(main_thread);

BAREOS_SCHEDULER.current = estack;

BareOSCallSwitch();

while(1);

//ENTRY point into

}



