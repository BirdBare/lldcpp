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
#include "buffer.h"

struct SpiObject
{
#ifdef USE_BAREOS
#endif

	struct RccObject rcc;

	uint8_t tx_dma_channel; 
	uint8_t rx_dma_channel;

	struct DmaObject *tx_dma_object;
	struct DmaObject *rx_dma_object;

	volatile SPI_TypeDef * const spi;

	struct
	{
		void *tx_data;
		void *rx_data;
		uint32_t tx_num_data;
		uint32_t rx_num_data;
	};

	struct SpiConfig *spi_config; //pointer to current configuration
	//end
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
	uint32_t clock_frequency; //spi clock frequency. config calculates be to actual

	void *interrupt_args; //arguments for interrupt
	void (*interrupt)(void *args); //respective spi is argument
																 //replaces default interrupt
																 //if not used then must be 0

	void *callback_args; //arguments callback function
	void (*callback)(void *args); //callback function for end of transfer

	struct GpioObject *slave_gpio_object;
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

			uint16_t:5; //padding to get the bits in the right spot for the registers

			uint16_t frame_format:1; //msb first or lsb first
#define FRAME_FORMAT_MSB 0
#define FRAME_FORMAT_LSB 1

			uint16_t:3; //padding to get the bits in the right spot for the registers

			uint16_t data_length:1; //length of the data in bits
#define DATA_LENGTH_8 0
#define DATA_LENGTH_16 0

			uint16_t:4; //padding to get the bits in the right spot for the registers
			//MSB
		};
	};
	
	uint16_t cr2; //options for the spi available to the user

//END DEVICE SETTINGS
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


uint32_t LldSpiConfigMaster(
	struct SpiObject *spi_object,
	struct SpiConfig *spi_config);


uint32_t LldSpiResetConfig(
	struct SpiObject * const spi_object);



//Polled
uint32_t LldSpiTransmitPolled(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t LldSpiTransferPolled(
	struct SpiObject *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t LldSpiReceivePolled(
	struct SpiObject *spi_object,
	void *data_in,
	uint32_t num_data);

//Interrupt
uint32_t LldSpiTransmitInterrupt(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t LldSpiTransferInterrupt(
	struct SpiObject *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t LldSpiReceiveInterrupt(
	struct SpiObject *spi_object,
	void *data_in,
	uint32_t num_data);


//Dma
uint32_t LldSpiTransmitDma(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t LldSpiTransferDma(
	struct SpiObject *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t LldSpiReceiveDma(
	struct SpiObject *spi_object,
	void *data_in,
	uint32_t num_data);




//######### WRITING OWN INTERRUPT FUNCTIONS

//High means transmit is ready to accept data
static inline uint32_t LldSpiTransmitReady(struct SpiObject *spi_object)
{
	return (spi_object->spi->SR & SPI_SR_TXE) && 
		(spi_object->spi->CR2 & SPI_CR2_TXEIE);
}

//high means receive is ready to give data
static inline uint32_t LldSpiReceiveReady(struct SpiObject *spi_object)
{
	return spi_object->spi->SR & SPI_SR_RXNE &&
		(spi_object->spi->CR2 & SPI_CR2_RXNEIE);
}

static inline void LldSpiTxDisableInterrupt(struct SpiObject *spi_object)
{
	spi_object->spi->CR2 &= ~SPI_CR2_TXEIE;
}

static inline void LldSpiRxDisableInterrupt(struct SpiObject *spi_object)
{
	spi_object->spi->CR2 &= ~SPI_CR2_RXNEIE;
}

void LldSpiPutDataObject(struct SpiObject *spi_object, uint32_t data);

uint32_t LldSpiGetDataObject(struct SpiObject *spi_object);

void LldSpiPutDataDevice(struct SpiObject *spi_object, uint32_t data);

uint32_t LldSpiGetDataDevice(struct SpiObject *spi_object);

uint32_t LldSpiTxDecrementNumData(struct SpiObject *spi_object);

uint32_t LldSpiRxDecrementNumData(struct SpiObject *spi_object);

static inline void LldSpiCallCallback(struct SpiObject *spi_object)
{
	if(spi_object->spi_config->callback != 0)
	{
		spi_object->spi_config->callback(spi_object->spi_config->callback_args);
	}
}
//#########################################




#endif








