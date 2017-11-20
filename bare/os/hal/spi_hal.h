//
//
//
//
//



#ifndef SPI_HAL_H
#define SPI_HAL_H


	//ALL DRIVERS CAN PERFORM ANY NON-BLOCKING CHECK NEEDED

//SpiConfig
	//Should configure clock speed with variable clock_frequency
	//REQUIRED VARIABLES
	// mode: transmit,transfer,receive mode(polled,interrupt,dma)
	// master: master enable
	// frame_format: lsb/msb
	// clock_phase
	// clock_polarity
	// interrupt: user interrupt enable
	// slave_gpio_port
	// slave_gpio_pin
	//can configure any other options needed.

//SpiResetConfig
	//should reset all peripheral registers to reset values

//SpiTransmit
	//only transmit data. not return any data. 
	//sent data is placed in variable data_out and num data in variable num_data

//SpiTransfer
	//transmit and receives data.
	//send and received data in dara_out and data_in. num data in num_data

//SpiReceive
	//only receives data.
	//received data in data_in and num data in num_data

//SpiStop
	//immediately stop the current spi transfer. used in cases of timeout




#include "bare_defines.h"
#include "spi_lld.h"

//REQUIRED FUNCTIONS
extern uint32_t (*SPI_TRANSMIT_FUNCTIONS[])(struct SpiObject *spi_object);
extern uint32_t (*SPI_TRANSFER_FUNCTIONS[])(struct SpiObject *spi_object);
extern uint32_t (*SPI_RECEIVE_FUNCTIONS[])(struct SpiObject *spi_object);
//END REQUIRED

static inline void SpiInit(struct SpiObject * const spi_object)
{
	LldSpiInit(spi_object);
}

static inline void SpiDeInit(struct SpiObject * const spi_object)
{
	LldSpiDeinit(spi_object);
}

uint32_t SpiConfig(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);

uint32_t SpiResetConfig(
	struct SpiObject * const spi_object);

uint32_t SpiTransmit(
	struct SpiObject *spi_object);

uint32_t SpiTransfer(
	struct SpiObject *spi_object);

uint32_t SpiReceive(
	struct SpiObject *spi_object);

uint32_t SpiStop(
	struct SpiObject *spi_object);


//OWN INTERRUPT FUNCTIONS
static inline void SpiPutDataObject(struct SpiObject *spi_object, uint32_t data)
{
	return LldSpiPutDataObject(spi_object,data);
}

static inline uint32_t SpiGetDataObject(struct SpiObject *spi_object)
{
	return LldSpiGetDataObject(spi_object);
}

static inline void SpiPutDataDevice(struct SpiObject *spi_object, uint32_t data)
{
	return LldSpiPutDataDevice(spi_object,data);
}

static inline uint32_t SpiGetDataDevice(struct SpiObject *spi_object)
{
	return LldSpiGetDataDevice(spi_object);
}
//END


#endif








