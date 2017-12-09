//
//
//
//
//


#include "gpio_hal.h"
#include "spi_hal.h"
#include "nvic_lld.h"

#define RESET_BIT 2
#define ENABLE_BIT 3
#define DATACOMMAND_BIT 4
#define DATA_BIT 2
#define CLOCK_BIT 4
#define LIGHT_BIT 7

struct Nokia5110Driver
{
	struct SpiObject *spi_object; //spi object driving the lcd

	uint8_t nokia_pins; //values seen on the nokia pins from RST:2 - LIGHT:7

	uint8_t data_left; 
	uint8_t count; //for use by driver function //count of bit we are on in data
	uint8_t data; //for use by driver function //data we are bit counting on

};

struct Nokia5110Driver nokia = {&SPI1_OBJECT, 0b1 << LIGHT_BIT};


void Nokia5110Interrupt(void *args)
{
	volatile struct Nokia5110Driver *nokia = args;

	if(SpiReceiveReady(nokia->spi_object) != 0)
	//make sure the interrupt is called because of tx time
	{
		GpioToggleOutput(nokia->spi_object->spi_config->slave_gpio_object,
			nokia->spi_object->spi_config->slave_gpio_pin);
			//flash data enable pin
		GpioToggleOutput(nokia->spi_object->spi_config->slave_gpio_object,
			nokia->spi_object->spi_config->slave_gpio_pin);
			//diable data enable pin

		uint32_t data;
		SpiGetDataDevice(nokia->spi_object, &data);
		//must read data if we are going to use rx interrupt. 
	
		if(nokia->data_left == 0 && nokia->count == 0 &&
			(nokia->nokia_pins & (0b1 << CLOCK_BIT)) != 0)
		{
			BREAK(5);
			SpiDisableRxInterrupt(nokia->spi_object);
		}
		//if data left is zero then this will be the last interrupt
		//if nokia->count is zero then all the bits on this byte has been sent
		//if clock bit is low then the data has been sent. We can disable
	}

	if(SpiTransmitReady(nokia->spi_object) != 0)
	//make sure the interrupt is called because of tx time
	{
		if((nokia->nokia_pins & (0b1 << CLOCK_BIT)) != 0)
		{
			nokia->nokia_pins &= ~(0b1 << CLOCK_BIT);
			//reset clock in bit

			if(nokia->count == 8)
			{
				nokia->count = 0;
				//reset to zero

				if(nokia->data_left == 0)
				{
					SpiDisableTxInterrupt(nokia->spi_object);
				}
				//if data left is zero then this will be the last interrupt
			}
			//if nokia->count equals 8 then we are one last bit and we need more data
		}
		//if clock bit is set then we need to reset it this round
		//we can also do checks here. 
		//if count is equal to 8 then we need to set it to zero to get more data
			//if data_left == 0 then we are all out of everything and we need to quit
		else
		{

			if(nokia->count++ == 0)
			{
				nokia->data_left = SpiGetDataObject(nokia->spi_object,&nokia->data);
			}
			//everytime the count is equal to 0 we need to get a new data
			//increment count everytime we send data from it.

			if((nokia->data & 0b1) != 0)
			{
				nokia->nokia_pins |= 0b1 << DATA_BIT;	
			}
			else
			{
				nokia->nokia_pins &= ~(0b1 << DATA_BIT);
			}
			//modify the data bit

			nokia->data >>= 1;
			//moves bits of data down one

			nokia->nokia_pins |= 0b1 << CLOCK_BIT;
			//set clock bit high to clock data in.
		}

		SpiPutDataDevice(nokia->spi_object,nokia->nokia_pins);
		//send the data
	}
}
void NokiaInit(void)
{
	NvicEnableInterrupt(SPI1_IRQn);

	gpio_config.pin = PIN_6;
	gpio_config.mode = MODE_OUTPUT;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);

	GpioSetOutput(&GPIOA_OBJECT,PIN_6);

	gpio_config.pin = PIN_5 | PIN_7;
	gpio_config.mode = MODE_ALTERNATE;
	gpio_config.alternate = ALTERNATE_5;
	gpio_config.pupd = PUPD_PD;
	GpioConfig(&GPIOA_OBJECT, &gpio_config);
	//config spi pins.
	//SPI EXPERIMENTAL

	SpiInit(&SPI1_OBJECT);
	//init spi

				uint8_t data[6] = {0b11111111};
				uint8_t trash[6];

	struct SpiConfig spi_config = { .master=1, .spi_mode = SPI_MODE_INTERRUPT,
		.slave_gpio_object = &GPIOA_OBJECT, .slave_gpio_pin = PIN_6,
		.clock_frequency = 300000, 
		.interrupt = &Nokia5110Interrupt, .args = &nokia};

	SpiConfig(&SPI1_OBJECT, &spi_config);
	//config spi1 for lowest clock speed and default settings
}

