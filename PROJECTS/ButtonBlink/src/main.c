//
//
//
//
//


#include "main.h"
#include "gpio_hal.h"
#include "spi_hal.h"
#include "nvic_lld.h"

void NMI_Handler(void)
{
BREAK(99);
}

void HardFault_Handler(void)
{
BREAK(98);
}

void MemManage_Handler(void)
{
BREAK(97);
}

void BusFault_Handler(void)
{
BREAK(96);
}

void UsageFault_Handler(void)
{
BREAK(95);
}


#define RESET_BIT 0
#define ENABLE_BIT 1
#define DATACOMMAND_BIT 2
#define DATA_BIT 3
#define CLOCK_BIT 4
#define LIGHT_BIT 5

struct Nokia5110Driver
{
	struct SpiObject *spi_object; //spi object driving the lcd

	uint8_t mode; //shift register vs direct output mode

	uint8_t nokia_pins; //values seen on the nokia pins from RST:2 - LIGHT:7
	uint8_t count; //for use by driver function
	uint8_t data; //for use by driver function

};


void Nokia5110Interrupt(void *args)
{
	struct Nokia5110Driver *nokia = args;

	if(SpiTransmitReady(nokia->spi_object) != 0)
	//make sure the interrupt is called because of
	{
		GpioToggleOutput(nokia->spi_object->spi_config->slave_gpio_object,
			nokia->spi_object->spi_config->slave_gpio_pin);
			//flash data enable pin
		GpioToggleOutput(nokia->spi_object->spi_config->slave_gpio_object,
			nokia->spi_object->spi_config->slave_gpio_pin);
			//diable data enable pin

		if(nokia->count == 8)
		{
			SpiGetDataObject(nokia->spi_object,&nokia->data);
			nokia->count = 0;
		}
		//everytime the count is equal to 8 we need to get a new data

		
		if((nokia->nokia_pins & (0b1 << CLOCK_BIT)) != 0)
		{
			nokia->nokia_pins &= ~(0b1 << CLOCK_BIT);
		}
		//if clock bit is set then we need to reset it this round
		else
		{
			if((nokia->data & 0b1) != 0)
			{
				nokia->nokia_pins |= 0b1 << DATA_BIT;	
			}
			else
			{
				nokia->nokia_pins &= ~(0b1 << DATA_BIT);
			}
			//modify the data bit

			nokia->nokia_pins |= 0b1 << CLOCK_BIT;
			//set clock bit high to clock data in.
		}

		
	}

}




#define LATCH_PIN PIN_6
#define LATCH_PORT GPIOA_OBJECT

volatile uint8_t rdata = 0;


int main(void)
{
	FlashEnableArt(&FLASH_OBJECT);
	struct FlashConfig flash_config = {5};
	FlashConfig(&FLASH_OBJECT,&flash_config);
	//Enable Art Controller and set wait states

	struct ClockConfig clock_config = {168000000,168000000,42000000,84000000};
	ClockConfig(&clock_config);
	//configure the cpu clocks

	RccEnableClock(&GPIOD_OBJECT.rcc);
	RccEnableClock(&GPIOA_OBJECT.rcc);
	//enable peripheral clock for GPIOA and GPIOD

	struct GpioConfig gpio_config = {0};
	//pin config struct

	gpio_config.pin = PIN_13;
	gpio_config.mode = MODE_OUTPUT;
	gpio_config.speed = SPEED_VHIGH;
	GpioConfig(&GPIOD_OBJECT, &gpio_config);
	//config Orange LED

	gpio_config.pin = PIN_0;
	gpio_config.mode = MODE_INPUT;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config push button as input for turning led on and off
	
	NvicEnableInterrupt(SPI1_IRQn);

	gpio_config.pin = PIN_6;
	gpio_config.mode = MODE_OUTPUT;

	GpioSetOutput(&GPIOA_OBJECT,PIN_6);

	gpio_config.pin = PIN_5 | PIN_7 | PIN_6;
	gpio_config.mode = MODE_ALTERNATE;
	gpio_config.alternate = ALTERNATE_5;
	gpio_config.pupd = PUPD_PD;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config spi pins.
	//SPI EXPERIMENTAL

	SpiInit(&SPI1_OBJECT);
	//init spi

				uint8_t data[6] =
				{0b10000001,0b10000001,0b10000001,0b10000001,0b10000001,0b10000001};

	struct SpiConfig spi_config = { .master=1, .spi_mode = SPI_MODE_INTERRUPT,
		.slave_gpio_object = &GPIOA_OBJECT, .slave_gpio_pin = PIN_6,
		.clock_frequency = 100000, .frame_format = 1, .crc_polynomial = 0};

	SpiConfig(&SPI1_OBJECT, &spi_config);
	//config spi1 for lowest clock speed and default settings

	while(1)
	{
	 
		if(GpioGetInput(&GPIOA_OBJECT, PIN_0) != 0)
		{
				GpioToggleOutput(&GPIOD_OBJECT, PIN_13);

				for(int i = 0; i < 50000000; i++)
					asm("");
		}
		//if input is pressed. blink LED
		else
		{
			GpioSetOutput(&GPIOD_OBJECT, PIN_13);
		}
		//if input is depressed. turn on LED

		
			SpiTransfer(&SPI1_OBJECT,data,data,6);

				for(int i = 0; i < 200000; i++)
				 asm volatile ("nop");

	}
	return 1;
}

