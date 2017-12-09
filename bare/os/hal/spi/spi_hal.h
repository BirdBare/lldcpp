//
//
//
//
//



#ifndef SPI_HAL_H
#define SPI_HAL_H


	//ALL DRIVERS CAN PERFORM ANY NON-BLOCKING OPERATION NEEDED
//LldSpiConfig
	//Should configure clock speed with variable clock_frequency
	//can configure any other options needed.

//LldSpiResetConfig
	//should reset all peripheral registers to reset values

//LldSpiGetClockSpeed
	//return config clock speed

//LldSpiTransmit
	//only transmit data. not return any data. 
	//sent data is placed in variable data_out and num data in variable num_data

//LldSpiTransfer
	//transmit and receives data.
	//send and received data in dara_out and data_in. num data in num_data

//LldSpiReceive
	//only receives data.
	//received data in data_in and num data in num_data

//LldSpiStop
	//immediately stop the current spi transfer. used in cases of timeout

//LldSpiWaitTransfer
	//wait for current transfer to finish.



#include "bare_defines.h"
#include "spi_lld.h"

static inline void SpiInit(struct SpiObject * const spi_object)
{
	LldSpiInit(spi_object);
}

static inline void SpiDeInit(struct SpiObject * const spi_object)
{
	LldSpiDeinit(spi_object);
}


uint32_t SpiResetConfig(
	struct SpiObject * const spi_object);

//Polled
uint32_t SpiConfigMasterPolled(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);

uint32_t SpiTransmitPolled(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t SpiTransferPolled(
	struct SpiObject *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t SpiReceivePolled(
	struct SpiObject *spi_object,
	void *data_in,
	uint32_t num_data);

//Polled
uint32_t SpiConfigMasterInterrupt(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);

uint32_t SpiTransmitInterrupt(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t SpiTransferInterrupt(
	struct SpiObject *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t SpiReceiveInterrupt(
	struct SpiObject *spi_object,
	void *data_in,
	uint32_t num_data);

//Polled
uint32_t SpiConfigMasterDma(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);

uint32_t SpiTransmitDma(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data);

uint32_t SpiTransferDma(
	struct SpiObject *spi_object,
	void *data_out,
	void *data_in,
	uint32_t num_data);

uint32_t SpiReceiveDma(
	struct SpiObject *spi_object,
	void *data_in,
	uint32_t num_data);



uint32_t SpiStop(
	struct SpiObject *spi_object);


//OWN INTERRUPT FUNCTIONS

//high means transmit is ready to accept data
static inline uint32_t SpiTransmitReady(struct SpiObject *spi_object)
{
	return LldSpiTransmitReady(spi_object);
}

//High means receive is ready to give data
static inline uint32_t SpiReceiveReady(struct SpiObject *spi_object)
{
	return LldSpiReceiveReady(spi_object);
}

static inline void SpiPutDataObject(struct SpiObject *spi_object, 
	uint32_t data)
{
	LldSpiPutDataObject(spi_object,data);
}

static inline uint32_t SpiGetDataObject(struct SpiObject *spi_object)
{
	return LldSpiGetDataObject(spi_object);
}

static inline void SpiPutDataDevice(struct SpiObject *spi_object, uint32_t data)
{
	LldSpiPutDataDevice(spi_object,data);
}

static inline uint32_t SpiGetDataDevice(struct SpiObject *spi_object)
{
	return LldSpiGetDataDevice(spi_object);
}

static inline void SpiTxDisableInterrupt(struct SpiObject *spi_object)
{
	LldSpiTxDisableInterrupt(spi_object);
}

static inline void SpiRxDisableInterrupt(struct SpiObject *spi_object)
{
	LldSpiRxDisableInterrupt(spi_object);
}

static inline uint32_t SpiTxDecrementNumData(struct SpiObject *spi_object)
{
	return LldSpiTxDecrementNumData(spi_object);
}

static inline uint32_t SpiRxDecrementNumData(struct SpiObject *spi_object)
{
	return LldSpiRxDecrementNumData(spi_object);
}
//END


#endif








