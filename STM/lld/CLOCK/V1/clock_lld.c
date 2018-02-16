//
//
//
//
//

#include "clock_lld.h"

volatile uint32_t CLOCK_SPEED[5] = {16000000,16000000,16000000,16000000,0}; 
//Clock Speeds for APB1, APB2, AHB, CPU clocks in that order
//Reset value is 16 Mhz for all clocks

volatile uint16_t CLOCK_PRESCALER[2] = {0,0};
//Clock Prescalers for APB1, and APB2 in that order


uint32_t LldClockResetConfig(void)
{
		RCC->CR |= RCC_CR_HSION;
		//enable hsi

		do
		{
			asm volatile ("");
		} while ((RCC->CR & RCC_CR_HSIRDY) == 0);
		//wait for ready flag

		RCC->CFGR &= ~(uint32_t)0b11;
		//switch system clock to hsi

		do
		{
			asm volatile ("");
		} while((RCC->CFGR & RCC_CFGR_SWS) != (0b00 << 2));
		//wait for switch

		RCC->CR = 0x83;
		RCC->PLLCFGR = 0x24003010;
		RCC->CFGR = 0;
		RCC->CIR = 0;
		RCC->BDCR = 0;
		RCC->CSR = 0;
		RCC->SSCGR = 0;
		RCC->PLLI2SCFGR = 0x20003000;
		//reset registers

		FLASH->ACR = 0;	
		//reset flash clock settings

		CLOCK_SPEED[APB1] = HSI_SPEED;
		CLOCK_SPEED[APB2] = HSI_SPEED;
		CLOCK_SPEED[AHB] = HSI_SPEED;
		CLOCK_SPEED[CPU] = HSI_SPEED;
		CLOCK_SPEED[USB] = 0;
		//reset clock speeds

		CLOCK_PRESCALER[APB1] = 0;
		CLOCK_PRESCALER[APB2] = 0;
		//reset clock prescalers

	return 0;
}



uint32_t LldClockConfig(const struct ClockConfig * const clock_config)
{
	LldClockResetConfig();

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

	FLASH->ACR = 0b11 << 11;
	FLASH->ACR = (cpu_speed / 30000000) | (0b111 << 8);
	//set flash wait states
	//assumes an input voltage of greater than 2.7v always

//######################################################

	uint32_t hse_speed = clock_config->hse_speed;
	//get hse_speed. 

	uint32_t pllcfgr = 0;
	//pll prescalers register

	uint32_t crystal_speed;
	//speed of the oscillator from which we derive all other speeds

//#########TURN ON CORRECT OSCILLATOR############
	if(hse_speed == 0)
	{
		crystal_speed = HSI_SPEED;
		//set oscillator speed
		
		//pll src is hsi
		//hsi enabled
		//hsi ready and selected as sys clock
		//hse disabled
	} 
	//configure for HSI
	else
	{
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

		RCC->CFGR |= (crystal_speed / 1000000) << 16 | 1;
		//set RTC prescaler. Must be 1MHZ so divide by itself. Set sys clock HSE

		do
		{
			asm volatile ("");
		} while((RCC->CFGR & RCC_CFGR_SWS) != (0b01 << 2));
		//wait for system to switch to hse as clock

		RCC->CR &= ~RCC_CR_HSION;
		//disable hsi
	}
	//configure for HSE
	//if user is using the HSE then we always choose that over HSI
//##############################################

	uint32_t counter = 0;

//#########CONFIGURE THE PLL###################
	if(cpu_speed != crystal_speed)
	{
		pllcfgr |= crystal_speed / 2000000;
		//set pll input as 1 so we can adjust as needed

		uint32_t vco_clock, temp = 1;
		//set needed variables. we use vco_clock
		//flag to indiciate if we cant find good cpu values

		do
		{
			counter += 2;
			//increment by two because PLLP increments by two

			vco_clock = cpu_speed * counter;
			//calculate vvo clock by cpu_speed * PLLP values
	
			if(vco_clock > VCO_MAX || counter > 8)
			{
				if(temp == 0)
				{
					return 1;
				}

				vco_clock = temp = counter = 0;
				//reset everything and start again with cpu priority instead
			}
		} while(vco_clock < VCO_MIN || 
			(temp != 0 && (vco_clock % (USB_SPEED)) != 0)); 
		//find correct VCO_CLOCK which satisfies usb and user cpu speed.
		//if we cannot find an agreement. then we satisfy user set cpu speed using
		//temp variable.


		cpu_speed = CLOCK_SPEED[CPU] = vco_clock / counter;
		//set actual cpu speed

		temp = (vco_clock / (USB_SPEED / 1000000) + 999999) / 1000000;
		//get usb prescaler. clever way of always rounding up if decimal is not zero

		CLOCK_SPEED[USB] = vco_clock / temp;
		//set actual usb speed.

		counter = (counter / 2) - 1;
		//reduce counter to correct register value

		RCC->PLLCFGR = pllcfgr | ((vco_clock / 2000000) << 6) | (counter << 16) | 
			(temp << 24);
		//set PLL CFGR regester

		RCC->CR |= RCC_CR_PLLON;
		//enable PLL

		do
		{
			asm volatile ("");
		} while ((RCC->CR & RCC_CR_PLLRDY) == 0);
		//wait for ready flag

		RCC->CFGR &= ~0b11;
		RCC->CFGR |= 0b10;
		//enable PLL as sys clock

		do
		{
			asm volatile ("");
		} while((RCC->CFGR & RCC_CFGR_SWS) != (0b10 << 2));
		//wait for switch
	}
//###############################################

	CLOCK_SPEED[CPU] = cpu_speed;
	//set cpu speed officially 

//#########SET PRESCALER FOR AHB, AND APB BUSSES###########
	uint32_t temp;
	//variable for clock_speed calculations

//~~~~Get and set AHB~~~~~~~
	temp = 1;
	counter = 0;
	
	while((ahb_speed * temp) < cpu_speed && counter < 8)
	{
		if(temp == 16)
		{
			temp <<= 1;
		}
		//special case in datasheet

		temp <<= 1;
		//muliples of two

		counter++;
		//increase counter for correct register setting
	}

	ahb_speed = CLOCK_SPEED[AHB] = cpu_speed / temp;
	//set actual ahb speed
	
	RCC->CFGR |= (counter + 0b0111) << 4;
	//set the ahb speed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~Get and set APB1~~~~~~~
	temp = 1;
	counter = 0;
	
	while((apb1_speed * temp) < ahb_speed && counter < 4)
	{
		temp <<= 1;
		//muliples of two

		counter++;
		//increase counter for correct register setting
	}

	CLOCK_SPEED[APB1] = ahb_speed / temp;	
	CLOCK_PRESCALER[APB1] = counter;
	//calculate actual apb1 speed and set apb1 prescaler for drivers

	RCC->CFGR |= (counter + 0b011) << 10;
	//set the apb1 speed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~Get and set APB2~~~~~~~
	temp = 1;
	counter = 0;
	
	while((apb2_speed * temp) < ahb_speed && counter < 4)
	{
		temp <<= 1;
		//muliples of two

		counter++;
		//increase counter for correct register setting
	}

	CLOCK_SPEED[APB2] = ahb_speed / temp;
	CLOCK_PRESCALER[APB2] = counter;
	//calculate actual apb2 speed and set apb2 prescaler for drivers

	RCC->CFGR |= (counter + 0b011) << 13;
	//set the apb2 speed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#########################################################

	return 0;
}
