//
//
//
//
//



#include "PWR_lld.h"
#include "PERIPH.h"

struct PWRxo
	PWRo = {{0x40,28},0,PWR};



void PWR_Config(struct PWRxo *PWRo, int CR1, int CR2, int CSR2)
{
	PWR->CR1 = (PWR->CR1 & 0b00000000000011110010000000000000) | CR1;
	PWR->CR2 = CR2;
	PWR->CSR2 = CSR2;
}


int PWR_EnableOverdrive(struct PWRxo *PWRo)
{
	if((RCC->CR & RCC_CR_PLLON) != 0 &&
		 (RCC->CFGR & RCC_CFGR_SWS) != (0b10 << 2))
	{
		PWR->CR1 |= PWR_CR1_ODEN;

		PERIPH_WaitTillSet(&PWR->CSR1,PWR_CSR1_ODRDY);
	
		PWR->CR1 |= PWR_CR1_ODSWEN;

		PERIPH_WaitTillSet(&PWR->CSR1,PWR_CSR1_ODSWRDY);

		return 0;
	}
	return PWR_ENABLE_CLOCK;
}


int PWR_DisableOverdrive(struct PWRxo *PWRo)
{
	if((RCC->CFGR & RCC_CFGR_SWS) != (0b10 << 2))
	{
		PWR->CR1 &= ~(PWR_CR1_ODEN | PWR_CR1_ODSWEN);	

		PERIPH_WaitTillReset(&PWR->CSR1,PWR_CSR1_ODSWRDY);

		return 0;
	}
	return PWR_ENABLE_CLOCK;
}











