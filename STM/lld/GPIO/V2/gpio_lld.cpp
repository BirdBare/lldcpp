//
//
//
//
//

#include "gpio_lld.hpp"


#ifdef GPIOA
struct GpioHal GPIOA_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOAEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOA};
#endif
#ifdef GPIOB
struct GpioHal GPIOB_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOBEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOB};
#endif
#ifdef GPIOC
struct GpioHal GPIOC_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOCEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOC};
#endif
#ifdef GPIOD
struct GpioHal GPIOD_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIODEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOD};
#endif
#ifdef GPIOE
struct GpioHal GPIOE_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOEEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOE};
#endif
#ifdef GPIOF
struct GpioHal GPIOF_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOFEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOF};
#endif
#ifdef GPIOG
struct GpioHal GPIOG_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOGEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOG};
#endif
#ifdef GPIOH
struct GpioHal GPIOH_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOHEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOH};
#endif
#ifdef GPIOI
struct GpioHal GPIOI_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOIEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOI};
#endif
#ifdef GPIOJ
struct GpioHal GPIOJ_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOJEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOJ};
#endif
#ifdef GPIOK
struct GpioHal GPIOK_HAL = {
	{&RCC->AHB1ENR, 
	RCC_AHB1ENR_GPIOKEN,
	 &RCC->AHB1RSTR,
	RCC_PERIPHERAL_BUS_AHB}, 
	GPIOK};
#endif


//******************************************************************************
//
//
//
//******************************************************************************
GpioObject::GpioObject(GpioHal &hal, uint32_t pins, const GpioSettings &settings)
: _hal(hal), _settings(settings), _pins(pins)
{
	if(_hal.used_pins == 0)
	{
		RccEnableClock(&_hal.rcc);
		_hal.gpio->OSPEEDR = 0xffffffff;
	}
	//if no pins are configured then we need to activate peripheral

	if((_pins & _hal.used_pins) != 0)
	{
		BREAK(0); 
		//kill program if already configured
	}
	//check if pins are already configured

	_hal.used_pins |= pins;
	//if everything is good then we add pins to the used pins
}
GpioObject::GpioObject(GpioHal &hal, GPIO_PIN pin, const GpioSettings &settings)
: GpioObject(hal, (uint32_t)pin, settings) {}


//******************************************************************************
//
//
//
//******************************************************************************
GpioObject::~GpioObject()
{
	_hal.used_pins &= ~_pins;
	//remove pins from list 
	
	if(_hal.used_pins == 0)
	{ 
		RccDisableClock(&_hal.rcc);
	}
	//if number of pins is zero then we can deactivate peripheral
}
//destructor for gpio object

//******************************************************************************
//
//
//
//******************************************************************************
void GpioObject::Config(GPIO_MODE mode)
{
	uint32_t set_mode = 0, set_type = 0, set_pupd = 0;
	//variables to hold pins set bits until the end when we write the registers

	uint32_t reset_1 = 0;
	//for resetting registers with one bit

	uint32_t reset_2 = 0;
	//for resetting registers with two bits

	uint32_t count = 0;
	//bit count for 16 bit register and alternate function
	
	volatile GPIO_TypeDef * const gpio_port = _hal.gpio;
	//get gpio port from object

	uint32_t pins = _pins;
	
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

			set_mode |= mode << count_2;		
			set_type |= _settings.type << count;		
			set_pupd |= _settings.pupd << count_2;		
			//collect the set pins config in variables for the final read modify write
			
			if(mode == GPIO_MODE_ALT)
			{
				count_2 = (count & 0b111) << 2;
				//get new count 2. Different for alternate function
				
				gpio_port->AFR[count >> 3] &= ~(0b1111 << ((count_2)));
				gpio_port->AFR[count >> 3] |= (_settings.alt << ((count_2)));
				//Reset and Set Pin Alternate Function
			}
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
}





