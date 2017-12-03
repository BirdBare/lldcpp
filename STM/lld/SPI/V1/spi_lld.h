//
//
//
//
//



#ifndef SPI_LLD_H
#define SPI_LLD_H

#include "bare_defines.h"
#include "rcc_lld.h"
#include "clock_lld.h"
#include "gpio_lld.h"
#include "dma_lld.h"

struct SpiObject
{
	struct RccObject rcc;

	uint8_t tx_dma_channel; 
	uint8_t rx_dma_channel;

	struct DmaObject *tx_dma_object;
	struct DmaObject *rx_dma_object;

	volatile SPI_TypeDef * const spi;

	struct SpiConfig *spi_config; //pointer to current configuration
};

extern struct SpiObject
	SPI1_OBJECT,
	SPI2_OBJECT,
	SPI3_OBJECT,
	SPI4_OBJECT,
	SPI5_OBJECT,
	SPI6_OBJECT;



//CONFIG STRUCTURE FOR THIS DEVICE DRIVER
struct SpiConfig
{
	uint32_t num_data; //num data to send.

	void *data_in; //pointer to the memory area holding the data to send
	void *data_out; //pointer to the memory area to receive the incoming data.

	uint32_t clock_frequency; //spi clock frequency. config calculates be to actual

	void (*interrupt)(struct SpiObject *spi_object); //respective spi is argument
																									 //replaces default interrupt
																									 //if not used then must be 0

	void (*callback)(void *callback_args); //callback function for end of transfer
	void *callback_args; //arguments for callback function

	struct GpioObject *slave_gpio_port;
	uint16_t slave_gpio_pin;

//POSSIBLE DEVICE SETTINGS
	uint16_t crc_polynomial; //crc polynomial register

	union
	{
		uint16_t cr1; //options for the spi available to the user
		
		struct
		{
			//LSB
			uint16_t clock_phase:1; //When the data is sampled. first or second edge
#define CLOCK_PHASE_FIRST 0 
#define CLOCK_PHASE_SECOND 1 

			uint16_t clock_polarity:1; //the voltage of the clock when starting
#define CLOCK_POLARITY_LOW 0
#define CLOCK_POLARITY_HIGH 1

			uint16_t master:1; //enables master mode on spi

			uint16_t:4; //padding to get the bits in the right spot for the registers

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
		uint8_t cr2; //options for the spi available to the user
		
		struct
		{
			//LSB
			//enable bits. set to enable the functionality
			uint8_t:2;

			uint8_t multimaster:1;	//multimaster capability on nss pin

			uint8_t :1;

			uint8_t ti:1; //enabled TI protocol. All settings are automatic
			uint8_t:3;
			//MSB
		};
	};
//END DEVICE SETTINGS

	uint8_t spi_mode; //hal spi mode
#define SPI_MODE_POLLED 0
#define SPI_MODE_DMA 1
#define SPI_MODE_INTERRUPT 2

};
// END CONFIG STRUCTURE




static inline void LldSpiInit(struct SpiObject * const spi_object)
{
	RccEnableClock(&spi_object->rcc);
	RccEnableClock(&spi_object->tx_dma_object->rcc);
	RccEnableClock(&spi_object->rx_dma_object->rcc);
}
static inline void LldSpiDeinit(struct SpiObject * const spi_object)
{
	RccDisableClock(&spi_object->rcc);
	RccDisableClock(&spi_object->tx_dma_object->rcc);
	RccDisableClock(&spi_object->rx_dma_object->rcc);
}

uint32_t LldSpiConfig(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);

uint32_t LldSpiResetConfig(
	struct SpiObject * const spi_object);

uint32_t LldSpiTransmit(
	struct SpiObject *spi_object);

uint32_t LldSpiTransfer(
	struct SpiObject *spi_object);

uint32_t LldSpiReceive(
	struct SpiObject *spi_object);



//######### WRITING OWN INTERRUPT FUNCTIONS

//High means transmit is ready to accept data
static inline uint32_t LldSpiTransmitReady(struct SpiObject *spi_object)
{
	return spi_object->spi->SR & SPI_SR_TXE;
}

//high means receive is ready to give data
static inline uint32_t LldSpiReceiveReady(struct SpiObject *spi_object)
{
	return spi_object->spi->SR & SPI_SR_RXNE;
}

void LldSpiPutDataObject(struct SpiObject *spi_object, uint32_t data);

uint32_t LldSpiGetDataObject(struct SpiObject *spi_object);

void LldSpiPutDataDevice(struct SpiObject *spi_object, uint32_t data);

uint32_t LldSpiGetDataDevice(struct SpiObject *spi_object);
//#########################################




#endif








