//
//
//
//
//


#include "rcc_lld.hpp"

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
uint32_t RccEnableClock(const struct RccHal * const rcc_object)
{
	uint32_t prevstate = *rcc_object->enable_register & rcc_object->peripheral_bit;
	//get previous state

	*rcc_object->enable_register |= rcc_object->peripheral_bit;
	//enable clock bit
	
	return prevstate;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccDisableClock(const struct RccHal * const rcc_object)
{
	*rcc_object->enable_register &= ~rcc_object->peripheral_bit;
	//disable clock bit
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void RccResetPeripheral(const struct RccHal * const rcc_object)
{
	*rcc_object->reset_register |= rcc_object->peripheral_bit;
	//enable reset bit

	*rcc_object->reset_register &= ~rcc_object->peripheral_bit;
	//disable reset bit
}

