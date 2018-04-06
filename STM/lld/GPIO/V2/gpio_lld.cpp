//
//
//
//
//

#include "gpio_lld.hpp"


#ifdef GPIOA
struct GpioHal GPIOA_HAL = {
	{0x30, 
	0,
	AHB}, 
	GPIOA};
#endif
#ifdef GPIOB
struct GpioHal GPIOB_HAL = {
	{0x30, 
	1,
	AHB}, 
	GPIOB};
#endif
#ifdef GPIOC
struct GpioHal GPIOC_HAL = {
	{0x30, 
	2,
	AHB}, 
	GPIOC};
#endif
#ifdef GPIOD
struct GpioHal GPIOD_HAL = {
	{0x30, 
	3,
	AHB}, 
	GPIOD};
#endif
#ifdef GPIOE
struct GpioHal GPIOE_HAL = {
	{0x30, 
	4,
	AHB}, 
	GPIOE};
#endif
#ifdef GPIOF
struct GpioHal GPIOF_HAL = {
	{0x30, 
	5,
	AHB}, 
	GPIOF};
#endif
#ifdef GPIOG
struct GpioHal GPIOG_HAL = {
	{0x30, 
	6,
	AHB}, 
	GPIOG};
#endif
#ifdef GPIOH
struct GpioHal GPIOH_HAL = {
	{0x30, 
	7,
	AHB}, 
	GPIOH};
#endif
#ifdef GPIOI
struct GpioHal GPIOI_HAL = {
	{0x30, 
	8,
	AHB}, 
	GPIOI};
#endif
#ifdef GPIOJ
struct GpioHal GPIOJ_HAL = {
	{0x30, 
	9,
	AHB}, 
	GPIOJ};
#endif
#ifdef GPIOK
struct GpioHal GPIOK_HAL = {
	{0x30, 
	10,
	AHB}, 
	GPIOK};
#endif




//******************************************************************************
//
//
//
//******************************************************************************
uint32_t GpioOutput::Config()
{
	uint32_t set_mode = 0, set_type = 0, set_pupd = 0;
	//variables to hold pins set bits until the end when we write the registers

	uint32_t reset_1 = 0;
	//for resetting registers with one bit

	uint32_t reset_2 = 0;
	//for resetting registers with two bits

	uint32_t count = 0;
	//bit count for 16 bit register

	volatile GPIO_TypeDef * const gpio_port = GetHal()->gpio;
	//get gpio port from object

	uint32_t pins = GetPins();

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

			set_mode |= GPIO_MODE_OUTPUT << count_2;		
			set_type |= _type << count;		
			set_pupd |= _pupd << count_2;		
			//collect the set pins config in variables for the final read modify write
			
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
	gpio_port->PUPDR &= reset_2;
	//reset settings for the newly configured pins

	gpio_port->MODER |= set_mode;
	gpio_port->OTYPER |= set_type;
	gpio_port->PUPDR |= set_pupd;
	//set the settings for the newly configured pins

	return 0;
	//success!!
}


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t GpioInput::Config()
{
	uint32_t set_mode = 0, set_pupd = 0;
	//variables to hold pins set bits until the end when we write the registers

	uint32_t reset_2 = 0;
	//for resetting registers with two bits

	uint32_t count_2 = 0;
	//bit count for 16 bit register

	volatile GPIO_TypeDef * const gpio_port = GetHal()->gpio;
	//get gpio port from object

	uint32_t pins = GetPins();

	do
	{
		if((pins & 0b1) != 0)
		{
			reset_2 |= 0b11 << count_2;
			//sets bits to reset the set pins config for two bits

			set_mode |= GPIO_MODE_INPUT << count_2;		
			set_pupd |= _pupd << count_2;		
			//collect the set pins config in variables for the final read modify write
			
		}
		//check if pins is set to be configured before configuring

		count_2 += 2;
		//increment count for next round of bitshifting

		pins >>= 1;
		//shift pins by one to get next pin

	} while (pins != 0);
	//continue running while more pins need to be set. If zero then no more pins

	reset_2 = ~reset_2;
	//complement reset bits to reset the newly configured pins

	gpio_port->MODER &= reset_2;
	gpio_port->PUPDR &= reset_2;
	//reset settings for the newly configured pins

	gpio_port->MODER |= set_mode;
	gpio_port->PUPDR |= set_pupd;
	//set the settings for the newly configured pins

	return 0;
	//success!!
}


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t GpioAlt::Config()
{
	uint32_t set_mode = 0, set_type = 0, set_pupd = 0;
	//variables to hold pins set bits until the end when we write the registers

	uint32_t reset_1 = 0;
	//for resetting registers with one bit

	uint32_t reset_2 = 0;
	//for resetting registers with two bits

	uint32_t count = 0;
	//bit count for 16 bit register and alternate function
	
	volatile GPIO_TypeDef * const gpio_port = GetHal()->gpio;
	//get gpio port from object

	uint32_t pins = GetPins();

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

			set_mode |= GPIO_MODE_ALT << count_2;		
			set_type |= _type << count;		
			set_pupd |= _pupd << count_2;		
			//collect the set pins config in variables for the final read modify write
			
			count_2 = (count & 0b111) << 2;
			//get new count 2. Different for alternate function
				
			gpio_port->AFR[count >> 3] &= ~(0b1111 << ((count_2)));
			gpio_port->AFR[count >> 3] |= (_alt << ((count_2)));
			//Reset and Set Pin Alternate Function
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
	gpio_port->PUPDR &= reset_2;
	//reset settings for the newly configured pins

	gpio_port->MODER |= set_mode;
	gpio_port->OTYPER |= set_type;
	gpio_port->PUPDR |= set_pupd;
	//set the settings for the newly configured pins

	return 0;
	//success!!
}


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t GpioAnalog::Config()
{
	uint32_t set_mode = 0;
	//variables to hold pins set bits until the end when we write the registers

	uint32_t reset_2 = 0;
	//for resetting registers with two bits

	uint32_t count_2 = 0;
	//bit count for 16 bit register

	volatile GPIO_TypeDef * const gpio_port = GetHal()->gpio;
	//get gpio port from object

	uint32_t pins = GetPins();

	do
	{
		if((pins & 0b1) != 0)
		{
			reset_2 |= 0b11 << count_2;
			//sets bits to reset the set pins config for two bits

			set_mode |= GPIO_MODE_ANALOG << count_2;		
			//collect the set pins config in variables for the final read modify write
		}
		//check if pins is set to be configured before configuring

		count_2 += 2;
		//increment count for next round of bitshifting

		pins >>= 1;
		//shift pins by one to get next pin

	} while (pins != 0);
	//continue running while more pins need to be set. If zero then no more pins

	reset_2 = ~reset_2;
	//complement reset bits to reset the newly configured pins

	gpio_port->MODER &= reset_2;
	//reset settings for the newly configured pins

	gpio_port->MODER |= set_mode;
	//set the settings for the newly configured pins

	return 0;
	//success!!
}






