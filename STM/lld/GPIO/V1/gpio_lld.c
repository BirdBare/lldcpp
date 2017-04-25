//
//
//
//
//

#include "gpio_lld.h"

#ifdef GPIOA
struct GpioObject GPIOA_OBJECT = {{0x30,0},0,GPIOA};
#endif
#ifdef GPIOB
struct GpioObject GPIOB_OBJECT = {{0x30,1},0,GPIOB};
#endif
#ifdef GPIOC
struct GpioObject GPIOC_OBJECT = {{0x30,2},0,GPIOC};
#endif
#ifdef GPIOD
struct GpioObject GPIOD_OBJECT = {{0x30,3},0,GPIOD};
#endif
#ifdef GPIOE
struct GpioObject GPIOE_OBJECT = {{0x30,4},0,GPIOE};
#endif
#ifdef GPIOF
struct GpioObject GPIOF_OBJECT = {{0x30,5},0,GPIOF};
#endif
#ifdef GPIOG
struct GpioObject GPIOG_OBJECT = {{0x30,6},0,GPIOG};
#endif
#ifdef GPIOH
struct GpioObject GPIOH_OBJECT = {{0x30,7},0,GPIOH};
#endif
#ifdef GPIOI
struct GpioObject GPIOI_OBJECT = {{0x30,8},0,GPIOI};
#endif
#ifdef GPIOJ
struct GpioObject GPIOJ_OBJECT = {{0x30,9},0,GPIOJ};
#endif
#ifdef GPIOK
struct GpioObject GPIOK_OBJECT = {{0x30,10},0,GPIOK};
#endif

//******************************************************************************
//	
//									GPIO_ConFig
//		return: 0		- Success
//						!0	- Fail -- Pins not set returned
//	
//******************************************************************************
uint32_t GpioConfig1(struct GpioObject * const gpio_object, uint32_t GPIO_PIN,
	const uint32_t GPIO_MODE, const uint32_t GPIO_OUTTYPE, 
	const uint32_t GPIO_OUTSPEED, const uint32_t GPIO_PUPD,
	const uint32_t GPIO_ALTERNATE)
{

	volatile GPIO_TypeDef * const port = gpio_object->gpio;

	uint32_t respins = ~gpio_object->used, setpins = 0, setpins2 = 0;
	//get old usedpins

	gpio_object->used |= GPIO_PIN;
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
				
				port->AFR[count >> 3] &= ~(0b1111 << ((count2)));
				port->AFR[count >> 3] |= (GPIO_ALTERNATE << ((count2)));
				//Sets Pin Alternate Function
			}
    }
  }
	port->MODER &= ~setpins2;
	port->OTYPER &= ~setpins;
	port->OSPEEDR &= ~setpins2;
	port->PUPDR &= ~setpins2;

	port->MODER |= MODE;
	port->OTYPER |= TYPE;
	port->OSPEEDR |= SPEED;
	port->PUPDR |= PUPD;

	return setpins;
}


uint32_t GpioConfig(struct GpioObject * const gpio_object,
	const struct GpioConfig * const gpio_config)
{

	uint32_t pins = gpio_config->pins;

	//checks if pins collide.
	{
		uint32_t setpins = gpio_object->used & pins;
		//and used pins and config pins to check collision

		if(setpins != 0)
			return setpins;
		//Pins collide if not zero. So return colliding pins to user for resolution.
	}
	//in brackets so setpins will be killed immediately to free register space

	gpio_object->used |= pins;
	//add new pins to used pins

	volatile GPIO_TypeDef * const gpio_port = gpio_object->gpio;
	//get gpio port from object

	uint32_t set_mode = 0, set_type = 0, set_speed = 0, set_pupd = 0;
	//variables to hold pins set bits until the end when we write the registers

	uint32_t reset_1 = 0;
	//for resetting registers with one bit

	uint32_t reset_2 = 0;
	//for resetting registers with two bits

	uint32_t count = 0;
	//bit count for 16 bit register and alternate function
	
	do
	{
		if((pins & 0b1) != 0)
		{
      uint32_t count_2 = count << 1;
			//bit count for 32 bit register

			reset_1 |= 0b1 << count;
			//sets bits to reset the set pins config for single bit

			reset_2 |= 0b11 << count_2;
			//sets bits to reset the set pins config for two bits

			set_mode |= gpio_config->mode << count_2;		
			set_type |= gpio_config->type << count;		
			set_speed |= gpio_config->speed << count_2;		
			set_pupd |= gpio_config->pupd << count_2;		
			//collect the set pins in variables for the final read modify write
			
			if(gpio_config->mode == GPIO_MODE_ALTERNATE)
			{
				count_2 = (count & 0b111) << 2;
				//get new count 2. Different for alternate function
				
				gpio_port->AFR[count >> 3] &= ~(0b1111 << ((count_2)));
				gpio_port->AFR[count >> 3] |= (gpio_config->alternate << ((count_2)));
				//Reset and Set Pin Alternate Function
			}
			//only run if pins are configured as alternate functions
		}
		//check if pins is set to be configured before configuring

		count++;
		//increment count for next round of bitshifting

		pins >>= 1;
		//shift pins by one to get next pin

	} while (pins != 0);
	//continue running while more pins need to be set. If zero then no more pins

	reset_2 = ~reset_2;
	reset_1 = ~reset_1;
	//complement reset bits to reset the newly configured pins

	gpio_port->MODER &= reset_2;
	gpio_port->OTYPER &= reset_1;
	gpio_port->OSPEEDR &= reset_2;
	gpio_port->PUPDR &= reset_2;
	//reset settings for the newly configured pins

	gpio_port->MODER |= set_mode;
	gpio_port->OTYPER |= set_type;
	gpio_port->OSPEEDR |= set_speed;
	gpio_port->PUPDR |= set_pupd;
	//set the settings for the newly configured pins

	return 0;
	//success!!
}






