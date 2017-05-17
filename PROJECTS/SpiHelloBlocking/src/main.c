//
//
//
//
//


//PA5 SPI1 CLK
//PA6 SPI1 MISO
//PA7 SPI1 MOSI
// ALT FUNCTION 5

#include "main.h"
#include "gpio_lld.h"
#include "spi_lld.h"

int main(void)
{

	RccEnableClock(&GPIOA_OBJECT.rcc);
	//enable GPIOA clock

	struct GpioConfig gpio_config = {
		.pin = PIN_5 | PIN_6 | PIN_7,
		.mode = MODE_ALTERNATE,
		.speed = SPEED_VHIGH,
		.alternate = ALTERNATE_5 };
	//enable spi pins with alternate function

	GpioConfig(&GPIOA_OBJECT, &gpio_config);	
	//configure the pins

	RccEnableClock(&SPI1_OBJECT.rcc);
	//enable SPI1 clock

	struct SpiConfig spi_config = {
	.mstr = MSTR_MASTER, .lsbfirst = LSBFIRST_MSB, .frf = FRF_TI};
	spi_config.baud_rate = 135000;
	//config general spi settings at clock rate of about 135Khz.

	SpiConfig(&SPI1_OBJECT, &spi_config);
	//config the SPI1



	uint8_t data = 0b01011010;
	struct CommunicationConfig transmission = {0,&data,1,0-1};

	while(1)
		SpiPut8Blocking(&SPI1_OBJECT, &transmission);
	//write hello to mosi pin.


	return 1;
}




