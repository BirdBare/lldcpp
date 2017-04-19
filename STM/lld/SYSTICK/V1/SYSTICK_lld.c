//
//
//
//
//



#include "SYSTICK_lld.h"
#include "KERNEL.h"



volatile uint32_t SysTick_Ticks = 0;
volatile uint32_t SysTick_TicksPerMilli = 1;


int SysTick_Config(int CTRL, uint32_t TicksPerInterrupt) 
{	
	if((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == 0)
	{
		SysTick->LOAD = TicksPerInterrupt; /*Sets reload value*/ 
  
		SysTick_TicksPerMilli = (_FCPU / 1000) / TicksPerInterrupt;

		SysTick->CTRL = CTRL; 

		SysTick->VAL = 0;

		return 0;
	}
	return SYSTICK_CONFIG_ENABLED;
}

int SysTick_ResetConfig(void)
{
	if((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == 0)
	{
		SysTick->CTRL = 0x4;
		return 0;
	}
	return SYSTICK_CONFIG_ENABLED;
}

void SysTick_Enable(void)
{
	SysTick->CTRL |= 0b1;
}
void SysTick_Disable(void)
{
	SysTick->CTRL &= ~0b1;
}


int SysTick_MilliSec(void)
{
  return SysTick_Ticks / SysTick_TicksPerMilli;
}

void DelayMilli(uint32_t Milli)
{
  struct THREAD *thread = SCHEDULER_CurrentThread(&SCHEDULER);

  uint32_t ref = thread->thread.temp1 = SysTick_MilliSec();

  thread->thread.temp2 = Milli;
  
  do
  {
    SCHEDULER_CallScheduler();
  } while((SysTick_MilliSec() - ref) < Milli);

  thread->thread.temp2 = 0;
}





