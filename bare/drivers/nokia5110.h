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

	uint8_t data; //for use by driver function //data we are bit counting on

	uint8_t tx_count; //for use by driver function //count of bit we are on in data
	uint8_t rx_count; //for use by driver function //count of bit we are on in data

	uint16_t tx_data_left;
	uint16_t rx_data_left;
};

struct Nokia5110Driver nokia = {&SPI1_OBJECT};


void Nokia5110Interrupt(void *args)
{
	volatile struct Nokia5110Driver *nokia = args;

	if(SpiReceiveReady(nokia->spi_object) != 0)
	//make sure the interrupt is called because of tx 
	{
		GpioToggleOutput(nokia->spi_object->spi_config->slave_gpio_object,
			nokia->spi_object->spi_config->slave_gpio_pin);
			//flash data enable pin
		GpioToggleOutput(nokia->spi_object->spi_config->slave_gpio_object,
			nokia->spi_object->spi_config->slave_gpio_pin);
			//disable data enable pin

		SpiGetDataDevice(nokia->spi_object);
		//must read data if we are going to use rx interrupt. 
	
		if(++nokia->rx_count == 16)
		//count is 16 for rx because tx counts every other clock pulse
		//rx counts every clock pulse
		{
			nokia->rx_count = 0;
			nokia->rx_data_left = SpiRxDecrementNumData(nokia->spi_object);
		}

		if(nokia->rx_data_left == 0 && nokia->rx_count == 0)
		{
			SpiRxDisableInterrupt(nokia->spi_object);
			SpiCallCallback(nokia->spi_object);
		}
		//if data left is zero then this will be the last interrupt
		//if nokia->count is zero then all the bits on this byte has been sent
		//if clock bit is low then the data has been sent. We can disable
	}

	if(SpiTransmitReady(nokia->spi_object) != 0)
	//make sure the interrupt is called because of rx 
	{
		if((nokia->nokia_pins & (0b1 << CLOCK_BIT)) != 0)
		{
			nokia->nokia_pins &= ~(0b1 << CLOCK_BIT);
			//reset clock in bit

			if(nokia->tx_count == 8)
			{
				nokia->tx_count = 0;
				//reset to zero

				if(nokia->tx_data_left == 0)
				{
					SpiTxDisableInterrupt(nokia->spi_object);
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

			if(nokia->tx_count++ == 0)
			{
				nokia->data = SpiGetDataObject(nokia->spi_object);
				//get data. 

				nokia->tx_data_left = SpiTxDecrementNumData(nokia->spi_object);
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

	struct GpioConfig gpio_config;

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

	struct SpiConfig spi_config = { .slave_gpio_object = &GPIOA_OBJECT, 
		.slave_gpio_pin = PIN_6, .clock_frequency = 300000, 
		.interrupt = &Nokia5110Interrupt, .interrupt_args = &nokia};

	SpiConfigMaster(&SPI1_OBJECT, &spi_config);
	//config spi1 for lowest clock speed and default settings
}

