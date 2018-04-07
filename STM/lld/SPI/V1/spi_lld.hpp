//
//
//
//
//



#ifndef SPI_LLD_H
#define SPI_LLD_H

#include "board.h"
#include "rcc_lld.h"
#include "nvic_lld.h"
#include "gpio_lld.hpp"
#include "dma_lld.hpp"



enum SPI_CLOCK_PHASE
{
	SPI_CLOCK_PHASE_FIRST = 0,
	SPI_CLOCK_PHASE_SECOND = 1
};

enum SPI_CLOCK_POLARITY
{
	SPI_CLOCK_POLARITY_LOW = 0,
	SPI_CLOCK_POLARITY_HIGH = 1
};

enum SPI_BIT_ORDER
{
	SPI_BIT_ORDER_MSB = 0,
	SPI_BIT_ORDER_LSB = 1
};

enum SPI_DATA_LENGTH
{
	SPI_DATA_LENGTH_8 = 0,
	SPI_DATA_LENGTH_16 = 1
};

//******************************************************************************
//
//
//
//******************************************************************************
class SpiObject;

struct SpiHal
{
	struct RccHal rcc;
	struct NvicHal nvic;

	uint8_t tx_dma_channel; 
	uint8_t rx_dma_channel;

	struct DmaHal *tx_dma_object;
	struct DmaHal *rx_dma_object;

	volatile SPI_TypeDef * const spi;

	class SpiObject *owner = 0;
	//object that owns this spi

	uint32_t num_owners = 0;

	uint16_t tx_num_data = 0;
	uint16_t rx_num_data = 0;
	//num data left. Used to stop the spi
};

extern struct SpiHal
	SPI1_HAL,
	SPI2_HAL,
	SPI3_HAL,
	SPI4_HAL,
	SPI5_HAL,
	SPI6_HAL;



//******************************************************************************
//
//
//
//******************************************************************************
struct SpiObjectSettings
{
			//LSB
			SPI_CLOCK_PHASE clock_phase:1; 
			//When the data is sampled. first or second edge

			SPI_CLOCK_POLARITY clock_polarity:1; 
			//the voltage of the clock when starting

			uint16_t:5; //padding to get the bits in the right spot for the registers

			SPI_BIT_ORDER bit_order:1; 
			//msb first or lsb first

			uint16_t:3; //padding to get the bits in the right spot for the registers

			SPI_DATA_LENGTH data_length:1; 
			//length of the data in bits

			uint16_t:4; //padding to get the bits in the right spot for the registers
			//MSB
};


//******************************************************************************
//
//
//
//******************************************************************************
class SpiObject
{
	SpiHal *_hal;

	void *interrupt_args = 0; //arguments for interrupt
	void (*interrupt)(void *args) = 0; //respective spi is argument
																 //replaces default interrupt
																 //if not used then must be 0

	void *callback_args = 0; //arguments callback function
	void (*callback)(void *args) = 0; //callback function for end of transfer

	uint32_t clock_frequency = 0; 
	//spi clock frequency. config calculates be to actual

	uint16_t crc_polynomial = 0; 
	//crc polynomial register

	SpiObjectSettings _settings = {
		SPI_CLOCK_PHASE_FIRST,
		SPI_CLOCK_POLARITY_LOW,
		SPI_BIT_ORDER_MSB,
		SPI_DATA_LENGTH_8};
	//settings for object
	

};


/*
static inline uint32_t LldSpiInit(struct SpiHal * const spi_object)
{
	NvicEnableHalInterrupt(&spi_object->nvic);
	RccEnableClock(&spi_object->rcc);

	LldDmaInit(spi_object->tx_dma_object);
	LldDmaInit(spi_object->rx_dma_object);

	return 0;
}
static inline uint32_t LldSpiDeinit(struct SpiHal * const spi_object)
{
	NvicDisableHalInterrupt(&spi_object->nvic);
	RccDisableClock(&spi_object->rcc);

	LldDmaDeinit(spi_object->tx_dma_object);
	LldDmaDeinit(spi_object->rx_dma_object);

	return 0;
}


uint32_t LldSpiConfigMaster(
	struct SpiHal *spi_object,
	struct SpiConfig *spi_config);


uint32_t LldSpiResetConfig(
	struct SpiHal * const spi_object);

uint32_t LldSpiStop(
	struct SpiHal * const spi_object);

//Polled
uint32_t LldSpiTransmitPolled(
	struct SpiHal *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t LldSpiTransferPolled(
	struct SpiHal *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t LldSpiReceivePolled(
	struct SpiHal *spi_object,
	void *data_in,
	uint32_t num_data);

//Interrupt
uint32_t LldSpiTransmitInterrupt(
	struct SpiHal *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t LldSpiTransferInterrupt(
	struct SpiHal *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t LldSpiReceiveInterrupt(
	struct SpiHal *spi_object,
	void *data_in,
	uint32_t num_data);


//Dma
uint32_t LldSpiTransmitDma(
	struct SpiHal *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t LldSpiTransferDma(
	struct SpiHal *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t LldSpiReceiveDma(
	struct SpiHal *spi_object,
	void *data_in,
	uint32_t num_data);




//######### WRITING OWN INTERRUPT FUNCTIONS

//High means transmit is ready to accept data
static inline uint32_t LldSpiTransmitReady(struct SpiHal *spi_object)
{
	return (spi_object->spi->SR & SPI_SR_TXE) && 
		(spi_object->spi->CR2 & SPI_CR2_TXEIE);
}

//high means receive is ready to give data
static inline uint32_t LldSpiReceiveReady(struct SpiHal *spi_object)
{
	return spi_object->spi->SR & SPI_SR_RXNE &&
		(spi_object->spi->CR2 & SPI_CR2_RXNEIE);
}

static inline void LldSpiTxDisableInterrupt(struct SpiHal *spi_object)
{
	spi_object->spi->CR2 &= ~SPI_CR2_TXEIE;
}

static inline void LldSpiRxDisableInterrupt(struct SpiHal *spi_object)
{
	spi_object->spi->CR2 &= ~SPI_CR2_RXNEIE;
}

void LldSpiPutDataHal(struct SpiHal *spi_object, uint32_t data);

uint32_t LldSpiGetDataHal(struct SpiHal *spi_object);

void LldSpiPutDataDevice(struct SpiHal *spi_object, uint32_t data);

uint32_t LldSpiGetDataDevice(struct SpiHal *spi_object);

uint32_t LldSpiTxDecrementNumData(struct SpiHal *spi_object);

uint32_t LldSpiRxDecrementNumData(struct SpiHal *spi_object);

static inline void LldSpiCallCallback(struct SpiHal *spi_object)
{
	if(spi_object->spi_config->callback != 0)
	{
		spi_object->spi_config->callback(spi_object->spi_config->callback_args);
	}
}
//#########################################

*/


#endif








