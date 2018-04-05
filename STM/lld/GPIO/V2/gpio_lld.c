//
//
//
//
//

#include "gpio_lld.h"


#ifdef GPIOA
struct GpioObject GPIOA_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 0,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOA};
#endif
#ifdef GPIOB
struct GpioObject GPIOB_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 1,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOB};
#endif
#ifdef GPIOC
struct GpioObject GPIOC_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 2,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOC};
#endif
#ifdef GPIOD
struct GpioObject GPIOD_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 3,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOD};
#endif
#ifdef GPIOE
struct GpioObject GPIOE_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 4,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOE};
#endif
#ifdef GPIOF
struct GpioObject GPIOF_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 5,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOF};
#endif
#ifdef GPIOG
struct GpioObject GPIOG_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 6,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOG};
#endif
#ifdef GPIOH
struct GpioObject GPIOH_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 7,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOH};
#endif
#ifdef GPIOI
struct GpioObject GPIOI_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 8,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOI};
#endif
#ifdef GPIOJ
struct GpioObject GPIOJ_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 9,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOJ};
#endif
#ifdef GPIOK
struct GpioObject GPIOK_OBJECT = {
	.rcc.reg_offset = 0x30, 
	.rcc.bit_offset = 10,
	.rcc.peripheral_bus = AHB, 
	.gpio = GPIOK};
#endif

//******************************************************************************
//	
//									GPIOConfig
//		return: 0		- Success
//						!0	- Fail -- Pins not set returned
//	
//******************************************************************************
uint32_t LldGpioConfig(struct GpioObject * const gpio_object,
	const struct GpioConfig * const gpio_config)
{
	uint32_t set_mode = 0, set_type = 0, set_speed = 0, set_pupd = 0;
	//variables to hold pins set bits until the end when we write the registers

	uint32_t reset_1 = 0;
	//for resetting registers with one bit

	uint32_t reset_2 = 0;
	//for resetting registers with two bits

	uint32_t count = 0;
	//bit count for 16 bit register and alternate function
	
	volatile GPIO_TypeDef * const gpio_port = gpio_object->gpio;
	//get gpio port from object

	uint32_t pins = gpio_config->pin;

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
			//collect the set pins config in variables for the final read modify write
			
			if(gpio_config->mode == MODE_ALTERNATE)
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






