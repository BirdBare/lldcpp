//
//
//
//
//


#include "rcc_lld.h"

volatile uint32_t CLOCK_SPEED[5] = {16000000,16000000,16000000,16000000,0};
//Clock Speeds for APB1, APB2, AHB, CPU clocks in that order
//Reset value is 16 Mhz for all clocks

volatile uint16_t CLOCK_PRESCALER[2] = {0,0};
//Clock Prescalers for APB1, and APB2 in that order

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t RccLldEnableClock(const struct RccLld * const rcc)
{
	uint32_t prevstate = *rcc->enable_register & rcc->peripheral_bit;
	//get previous state

	*rcc->enable_register |= rcc->peripheral_bit;
	//enable clock bit
	
	return prevstate;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccLldDisableClock(const struct RccLld * const rcc)
{
	*rcc->enable_register &= ~rcc->peripheral_bit;
	//disable clock bit
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccLldResetPeripheral(const struct RccLld * const rcc)
{
	*rcc->reset_register |= rcc->peripheral_bit;
	//enable reset bit

	*rcc->reset_register &= ~rcc->peripheral_bit;
	//disable reset bit
}

