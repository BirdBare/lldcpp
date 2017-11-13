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
#include "buffer.h"

struct SpiControl;

struct SpiObject
{
	struct RccObject rcc;

	uint8_t tx_dma_channel; 
	uint8_t rx_dma_channel;

	struct DmaObject *tx_dma_object;
	struct DmaObject *rx_dma_object;

	volatile SPI_TypeDef * const spi;

	struct SpiConfig *spi_config; //pointer to current configuration

	//MUTEX WOULD GO HERE

};

extern struct SpiObject
	SPI1_OBJECT,
	SPI2_OBJECT,
	SPI3_OBJECT,
	SPI4_OBJECT,
	SPI5_OBJECT,
	SPI6_OBJECT;



struct SpiConfig
{
//###########ACTUAL SPI CONFIG OPTIONS
	uint32_t clock_frequency; //spi clock frequency. calculated in config to actual
//####################################

//##############SPI TRANSFER SETTINGS
	uint32_t num_data; //num data to send.

	void *data_in; //pointer to the memory area holding the data to send
	void *data_out; //pointer to the memory area to receive the incoming data.

	uint32_t crc_polynomial; //crc polynomial register

	void (*interrupt)(struct SpiObject *spi_object); //respective spi is argument
																									 //replaces default interrupt

	union
	{
		uint16_t cr1; //options for the spi available to the user
		
		struct
		{
			//LSB
			uint16_t clock_phase:1; //When the data is sampled. first or second edge
#define CLOCK_PHASE_FIRST 0 
#define CLOCK_PHASE_SECOND 1 

			uint16_t clock_idle_polarity:1; //the voltage of the clock when starting
#define CLOCK_IDLE_POLARITY_LOW 0
#define CLOCK_IDLE_POLARITY_HIGH 1

			uint16_t:5; //padding to get the bits in the right spot for the registers

			uint16_t frame_format:1; //msb first or lsb first
#define FRAME_FORMAT_MSB 0
#define FRAME_FORMAT_LSB 1

			uint16_t:3; //padding to get the bits in the right spot for the registers

			uint16_t data_length:1; //length of the data in bits
#define DATA_LENGTH_8 0
#define DATA_LENGTH_16 0

			uint16_t:3; //padding to get the bits in the right spot for the registers
			uint16_t bidirectional_mode:1; //enables the ability to use one pin
																		 //not currently working for receive
			//MSB
		};
	};
	
	union
	{
		uint16_t cr2; //options for the spi available to the user
		
		struct
		{
			//LSB
			//enable bits. set to enable the functionality
			uint16_t:2;

			uint16_t multimaster_disable:1;	//multimaster capability on nss pin

			uint16_t :1;

			uint16_t ti_mode:1; //enabled TI protocol. All settings are automatic
			uint16_t error_interrupt:1; //enables the error interrupt

			uint16_t:10; //padding to get the bits in the right spot for the registers
			//MSB
		};
	};
//#################################
};

uint32_t SpiConfigMaster(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);

uint32_t SpiResetConfig(
	const struct SpiObject * const spi_object);

uint32_t SpiTransmitPolled(
	struct SpiObject *spi_object);

uint32_t SpiTransferPolled(
	struct SpiObject *spi_object);

//uint32_t SpiReceivePolled(
	//struct SpiObject *spi_object);

uint32_t SpiTransmitDma(
	struct SpiObject *spi_object);

uint32_t SpiTransferDma(
	struct SpiObject *spi_object);

//uint32_t SpiReceiveDma(
	//struct SpiObject *spi_object);

uint32_t SpiTransmitInterrupt(
	struct SpiObject *spi_object);








#endif








