//
//
//
//
//



#include "systick_lld.h"

volatile uint32_t systick_milli = 0;


void SysTick_Handler(void)
{
	++systick_milli;
}

void SysTickDelayMilli(uint32_t milli)
{
  const uint32_t milli_ref = SysTickGetMilli();

  do
  {
		WAIT;
  } while((SysTickGetMilli() - milli_ref) < milli);
}





