//
//
//
//
//

#include "clock_lld.h"

volatile uint32_t CLOCK_SPEED[4] = {16,16,16,16}; 
//Clock Speeds for CPU, AHB, APB1, APB2 clocks in that order
//Reset value is 16 Mhz for all clocks

uint32_t ClockConfig(const struct ClockConfig * const clock_config)
{
//######CHECKS#########################################
	uint32_t cpu_speed = clock_config->cpu_speed;
	uint32_t ahb_speed = clock_config->ahb_speed;
	uint32_t apb1_speed = clock_config->apb1_speed;
	uint32_t apb2_speed = clock_config->apb2_speed;

	if(cpu_speed > CPU_MAX || ahb_speed > AHB_MAX || apb1_speed > APB1_MAX || 
		apb2_speed > APB2_MAX)
	{
		return 1;
	}
	//check clocks arent too high

//######################################################

	uint32_t hse_speed = clock_config->hse_speed;
	//get hse_speed. 

	uint32_t rcccfgr = 0;
	//peripheral bus speeds register to config

	uint32_t pllcfgr = 0;
	//pll prescalers register

	uint32_t crystal_speed;
	//speed of the oscillator from which we derive all other speeds

//#########TURN ON CORRECT OSCILLATOR############
	if(hse_speed == 0)
	{
		crystal_speed = HSI_SPEED;
		//set oscillator speed
		
		pllcfgr = 0;
		//set pll source HSI
		
		RCC->CR |= RCC_CR_HSION;
		//enable hsi

		do
		{
			asm volatile ("");
		} while ((RCC->CR & RCC_CR_HSIRDY) == 0);
		//wait for ready flag

		RCC->CR &= ~RCC_CR_HSEON;
		//disable hse
	} else
	{
		rcccfgr |= hse_speed << 16 | 1;
		//set RTC prescaler. Must be 1MHZ so divide by itself. Set sys clock HSE

		crystal_speed = hse_speed;
		//set oscillator speed as hse speed

		pllcfgr = RCC_PLLCFGR_PLLSRC;
		//set pll source HSE

		RCC->CR |= RCC_CR_HSEON;
		//enable hse

		do
		{
			asm volatile ("");
		} while ((RCC->CR & RCC_CR_HSERDY) == 0);
		//wait for ready flag

		RCC->CR &= ~RCC_CR_HSION;
		//disable hsi
	}
	//if user is using the HSE then we always choose that over HSI
//##############################################

//#########CONFIGURE THE PLL###################
	
		pllcfgr |= crystal_speed;
		//set pll input as 1 so we can adjust as needed

		uint32_t counter = 0;
		uint32_t vco_clock;
		//set needed variables. we use vco_clock

		do
		{
			counter += 2;
			//increment by two because PLLP increments by two

			vco_clock = cpu_speed * counter;
			//calculate vvo clock by cpu_speed * PLLP values
	
			if(vco_clock > VCO_MAX || counter > 8)
			{
				vco_clock = ((cpu_speed / USB_SPEED) * USB_SPEED) << 1;
				//integer math will chop the decimal then we multplu by two since the
				//sys clock has to divide by two. This is a very ruff approximation.

				counter = 2;
			}
		} while((vco_clock % USB_SPEED) != 0); 
		//find correct VCO_CLOCK which satisfies usb and user cpu speed.
		//if we cannot find an agreement. we will always satisfy usb clock.

		CLOCK_SPEED[CPU] = vco_clock / counter;
		//set actual cpu speed

		counter = (counter / 2) - 1;
		//reduce counter to correct register value

		RCC->PLLCFGR = pllcfgr | (vco_clock << 6) | (counter << 16) | 
			((vco_clock / USB_SPEED) << 24);
		//set PLL CFGR regester

		RCC->CR |= RCC_CR_PLLON;
		//enable PLL

		do
		{
			asm volatile ("");
		} while ((RCC->CR & RCC_CR_PLLRDY) == 0);
		//wait for ready flag

		rcccfgr &= ~0b11;
		rcccfgr |= 0b10;
		//enable PLL as sys clock
//###############################################

//#########SET PRESCALER FOR AHB, AND APB BUSSES###########
	uint32_t temp;
	//variable for clock_speed calculations

	cpu_speed = ClockGetSpeed(CPU);
	//set new cpu speed

	if(ahb_speed > cpu_speed)
	{
		ahb_speed = cpu_speed;
	}
	//special case if cpu speed gets reduced below ahb bus speed

//~~~~Get and set AHB~~~~~~~
	temp = cpu_speed / ahb_speed;
	CLOCK_SPEED[AHB] = cpu_speed / temp;
	//calculate actual apb1 speed

	rcccfgr |= (LogBase2(temp) + 0b0111) << 4;
	//set the ahb speed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~

ahb_speed = ClockGetSpeed(AHB);
//set actual ahb speed

if(apb2_speed > ahb_speed)
	{
		apb2_speed = ahb_speed;
	}
	//special case if ahb speed gets reduced below apb2 bus speed
	//apb1 not checked because ahb must always be atleast 48MHz

//~~~~Get and set APB1~~~~~~~
	temp = ahb_speed / apb1_speed;
	CLOCK_SPEED[APB1] = ahb_speed / temp;	
	//calculate actual apb1 speed

	if(CLOCK_SPEED[APB1] > apb1_speed)
	{
		CLOCK_SPEED[APB1] = ahb_speed / ++temp;
	}
	//check actual speed is less than or equal to the set speed to protect the bus

	rcccfgr |= (LogBase2(temp) + 0b011) << 10;
	//set the apb1 speed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~Get and set APB2~~~~~~~
	temp = ahb_speed / apb2_speed;
	CLOCK_SPEED[APB2] = ahb_speed / temp;
	//calculate actual apb2 speed

	if(CLOCK_SPEED[APB2] > apb2_speed)
	{
		CLOCK_SPEED[APB2] = ahb_speed / ++temp;
	}
	//check actual speed is less than or equal to the set speed to protect the bus

	rcccfgr |= (LogBase2(temp) + 0b011) << 13;
	//set the apb2 speed

	CLOCK_SPEED[CPU] *= 1000000;
	CLOCK_SPEED[AHB] *= 1000000;
	CLOCK_SPEED[APB1] *= 1000000;
	CLOCK_SPEED[APB2] *= 1000000;
	//finish by multiplying the speeds by 1000000 to convert to Mhz

//~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#########################################################

//#############SWITCH SYSTEM CLOCK########################
	RCC->CFGR = rcccfgr;
	//Set rcccfgr

	do
	{
		asm volatile ("");
	} while((RCC->CFGR & RCC_CFGR_SWS) != (0b10 << 2));
	//wait for switch
//#########################################################

return 0;

}
