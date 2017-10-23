//
//
//
//
//



#ifndef SPI_H
#define SPI_H

#include "bare_defines.h"
#include "rcc_lld.h"
#include "clock_lld.h"
#include "gpio_lld.h"
#include "dma_lld.h"

struct SpiControl;

struct SpiObject
{
	struct RccObject rcc;

	uint8_t tx_dma_channel; 
	uint8_t rx_dma_channel;

	struct DmaObject *tx_dma_object;
	struct DmaObject *rx_dma_object;

	volatile SPI_TypeDef * const spi;

	struct SpiControl * spi_control;

	void (*interrupt)(struct SpiObject *spi_object); //respective spi is argument
};

extern struct SpiObject
	SPI1_OBJECT,
	SPI2_OBJECT,
	SPI3_OBJECT,
	SPI4_OBJECT,
	SPI5_OBJECT,
	SPI6_OBJECT;

struct SpiControl
{
	union
	{
		struct //master control
		{
			struct GpioObject * slave_gpio_object;
			uint16_t slave_pin;
			uint16_t:16;
		};

		struct //slave control
		{

		};
	};
};
//object the spi will use to control the slave

struct SpiConfig
{

	union
	{
		uint16_t options; //options for the spi available to the user
		//list of options by bit from lsb to msb
		//LSB  CP:CIP:3:FF:3:DL:2:TM:EI:TI:RI  MSB
		
		struct
		{
			//LSB
			uint16_t clock_phase:1; //When the data is sampled. first or second edge
#define CLOCK_PHASE_FIRST 0 
#define CLOCK_PHASE_SECOND 1 

			uint16_t clock_idle_polarity:1; //the voltage of the clock when starting
#define CLOCK_IDLE_POLARITY_LOW 0
#define CLOCK_IDLE_POLARITY_HIGH 1

			//enable bits. set to enable the functionality
			uint16_t ti_mode:1; //enabled TI protocol. All settings are automatic
			uint16_t error_interrupt:1; //enables the error interrupt
			uint16_t rx_interrupt:1; //enables the rx data received interrupt
			uint16_t tx_interrupt:1; //enables the tx empty interrupt

			uint16_t:1; //padding to get the bits in the right spot for the registers

			uint16_t frame_format:1; //msb first or lsb first
#define FRAME_FORMAT_MSB 0
#define FRAME_FORMAT_LSB 1

			uint16_t:3; //padding to get the bits in the right spot for the registers

			uint16_t data_length:1; //length of the data in bits
#define DATA_LENGTH_8 0
#define DATA_LENGTH_16 0

			uint16_t:3; //padding to get the bits in the right spot for the registers
			uint16_t bidirectional_mode:1;
			//MSB
		};
	};

	uint16_t crc_polynomial; //crc polynomial register

	uint32_t clock_frequency; //spi clock frequency. calculated in config to actual

	void (*interrupt)(struct SpiObject *spi_object); //respective spi is argument

	struct SpiControl spi_control;
};

uint32_t SpiConfigMaster(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);

uint32_t SpiResetConfig(
	const struct SpiObject * const spi_object);

uint32_t SpiTransmitPolled(
	struct SpiObject *spi_object,
	uint32_t num_data,
	void *data_out);

uint32_t SpiTransferPolled(
	struct SpiObject *spi_object,
	uint32_t num_data,
	void *data_out,
	void *data_in);







#endif








