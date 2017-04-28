//
//
//
//
//

#ifndef HSE_CLOCK_SPEED
	#define HSE_CLOCK_SPEED 0
#endif

#include "clock_lld.h"

uint32_t CLOCK_SPEED[4] = {16000000,16000000,16000000,16000000}; 
//Clock Speeds for CPU, AHB, APB1, APB2 clocks in that order
//Reset value is 16 Mhz for all clocks

const uint16_t ahb_divider[16] = {1,1,1,1,1,1,1,1,2,4,8,16,64,128,256,512};
const uint8_t apb_divider[8] = {1,1,1,1,2,4,8,16};
const uint8_t pllp_divider[4] ={2,4,6,8};

#define CLOCKCONFIG_PLL_ERROR 0b11

uint32_t ClockConfig(
const struct ClockConfig * const clock_config)
{
	const uint32_t source = clock_config->sw | clock_config->pllsrc;
	//gets sw and pllsrc to decide if we enable HSI or HSE

	if(((source & 0b10) & (RCC->CFGR & 0b10)) != 0) 
	//if not zero, PLL is on and we are trying to config PLL. PROBLEM!
	{
		return CLOCKCONFIG_PLL_ERROR;
		//return with error because we cannot config PLL while it is on.
	}

	if((source & 1) == 0) //get bit 0, if zero means we use HSI for something
	{
		RCC->CR |= RCC_CR_HSION;
		//enable HSI oscillator

		while((RCC->CR & RCC_CR_HSIRDY) == 0)
			WAIT;
		//wait for HSI to be ready and stable

		CLOCK_SPEED[0] = 16000000;
		//set clock speed value
	}
	else //if not zero, we use HSE for something
	{
		RCC->CR |= RCC_CR_HSEON;
		//enable HSE oscillator

		while((RCC->CR & RCC_CR_HSERDY) == 0)
			WAIT;
		//wait for HSE to be ready and stable

		CLOCK_SPEED[0] = HSE_CLOCK_SPEED;
		//set clock speed value
	}

	if((source & 0b10) != 0) //this means we use PLL as clock source
	{
		RCC->PLLCFGR = clock_config->pllcfgr;
		//Set PLLCFGR settings in register

		RCC->CR |= RCC_CR_PLLON;
		//turn on the PLL
		
		while((RCC->CR & RCC_CR_PLLRDY) == 0)
			WAIT;
		//wait for PLL to be locked

		CLOCK_SPEED[0] = CLOCK_SPEED[0] / clock_config->pllm * clock_config->plln 
											/ pllp_divider[clock_config->pllp];
		//Update CPU clock Speed if we are using PLL
	}
	else
	{
		RCC->CR &= ~RCC_CR_PLLON;
	}

	RCC->CFGR = clock_config->cfgr;
	//set CFGR register to make switch happen

	if((source & 1) == 0) //source is HSI
	{
		RCC->CR &= ~RCC_CR_HSEON;
		//disable HSE because we are not using it
	}
	else //source is HSE
	{
		RCC->CR &= ~RCC_CR_HSION;
		//disable HSI because we are not using it
	}
	
	
	CLOCK_SPEED[1] = CLOCK_SPEED[0] / ahb_divider[clock_config->hpre];
	//AHB

	CLOCK_SPEED[2] = CLOCK_SPEED[1] / apb_divider[clock_config->ppre1];
	//APB1

	CLOCK_SPEED[3] = CLOCK_SPEED[1] / apb_divider[clock_config->ppre2];
	//APB2

	//set new clock speed values for AHB, APB1, and APB2


	return RCC->CFGR & RCC_CFGR_SWS;
	//return SWS in CFGR register so user can check proper clock source.
}

