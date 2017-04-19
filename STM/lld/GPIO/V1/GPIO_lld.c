//
//
//
//
//

#include "GPIO_lld.h"

#ifdef GPIOA
	struct GPIOxo GPIOAo = {{0x30,0},0,GPIOA};
#endif
#ifdef GPIOB
	struct GPIOxo GPIOBo = {{0x30,1},0,GPIOB};
#endif
#ifdef GPIOC
	struct GPIOxo GPIOCo = {{0x30,2},0,GPIOC};
#endif
#ifdef GPIOD
	struct GPIOxo GPIODo = {{0x30,3},0,GPIOD};
#endif
#ifdef GPIOE
	struct GPIOxo GPIOEo = {{0x30,4},0,GPIOE};
#endif
#ifdef GPIOF
	struct GPIOxo GPIOFo = {{0x30,5},0,GPIOF};
#endif
#ifdef GPIOG
	struct GPIOxo GPIOGo = {{0x30,6},0,GPIOG};
#endif
#ifdef GPIOH
	struct GPIOxo GPIOHo = {{0x30,7},0,GPIOH};
#endif
#ifdef GPIOI
	struct GPIOxo GPIOIo = {{0x30,8},0,GPIOI};
#endif
#ifdef GPIOJ
	struct GPIOxo GPIOJo = {{0x30,9},0,GPIOJ};
#endif
#ifdef GPIOK
	struct GPIOxo GPIOKo = {{0x30,10},0,GPIOK};
#endif

//******************************************************************************
//	
//									GPIO_ConFig
//		return: 0		- Success
//						!0	- Fail -- Pins not set returned
//	
//******************************************************************************
uint32_t GPIO_Config(volatile struct GPIOxo * const GPIOo, uint32_t GPIO_PIN,
	const uint32_t GPIO_MODE, const uint32_t GPIO_OUTTYPE, 
	const uint32_t GPIO_OUTSPEED, const uint32_t GPIO_PUPD,
	const uint32_t GPIO_ALTFUNCTION)
{

	volatile GPIO_TypeDef * const GPIOx = GPIOo->GPIOx;

	uint32_t respins = ~GPIOo->used, setpins = 0, setpins2 = 0;
	//get old usedpins

	GPIOo->used |= GPIO_PIN;
	//update usedpins

	uint32_t MODE = 0, TYPE = 0, SPEED = 0, PUPD = 0;

  for(uint8_t count = 0; count < 16; count++)
  {
    if((GPIO_PIN & ((uint32_t)0b1 << count) & respins) != 0)
    {
      uint32_t count2 = count << 1;

			//indicates used or reserved pins. will be stored and returned.
			setpins |= ((uint32_t)0b1 << count);
			setpins2 |= ((uint32_t)0b11 << count2);
    
      MODE |= (GPIO_MODE << (count2)); 
      //
      TYPE |= (GPIO_OUTTYPE << count);
      //      
      SPEED |= (GPIO_OUTSPEED << (count2)); 
      //
      PUPD |= (GPIO_PUPD << (count2)); 
      //  
      

			if(GPIO_MODE == GPIO_MODE_ALTERNATE)
			{
				count2 = (count & 0b111) << 2;
				//
				
				GPIOx->AFR[count >> 3] &= ~(0b1111 << ((count2)));
				GPIOx->AFR[count >> 3] |= (GPIO_ALTFUNCTION << ((count2)));
				//Sets Pin Alternate Function
			}
    }
  }
	GPIOx->MODER &= ~setpins2;
	GPIOx->OTYPER &= ~setpins;
	GPIOx->OSPEEDR &= ~setpins2;
	GPIOx->PUPDR &= ~setpins2;

	GPIOx->MODER |= MODE;
	GPIOx->OTYPER |= TYPE;
	GPIOx->OSPEEDR |= SPEED;
	GPIOx->PUPDR |= PUPD;

	return setpins;
}


