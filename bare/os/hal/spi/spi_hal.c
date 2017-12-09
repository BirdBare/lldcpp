//
//
//
//
//



#include "spi_hal.h"

//
// SPI RESET CONFIG
//
uint32_t SpiResetConfig(
	struct SpiObject * const spi_object)
{
	return LldSpiResetConfig(spi_object);
}


//
// SPI_CONFIG POLLED
//
uint32_t SpiConfigMasterPolled(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config)
{
	if(spi_object->spi_config != 0)
	{
		return 1;
	}
	//check if already configured

	LldSpiResetConfig(spi_object);
	//reset the configuration before we config as a precaution

	uint32_t ret = LldSpiConfigMasterPolled(spi_object, spi_config);
	//configure the spi to be used.

	spi_object->spi_config = spi_config;
	//set spi as configured.

	return ret;
}


//
// SPI TRANSMIT POLLED
//
uint32_t SpiTransmitPolled(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data)
{
return LldSpiTransmitPolled(spi_object,data_out,num_data);
}

//
// SPI TRANSFER POLLED 
//
uint32_t SpiTransferPolled(
	struct SpiObject *spi_object, 
	void *data_out,
	void *data_in,
	uint32_t num_data)
{
return LldSpiTransferPolled(spi_object,data_out,data_in,num_data);
}



//
// SPI Receive POLLED 
//
uint32_t SpiReceivePolled(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	return LldSpiReceivePolled(spi_object,data_in,num_data);
}




//
// SPI_CONFIG INTERRUPT
//
uint32_t SpiConfigMasterInterrupt(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config)
{
	if(spi_object->spi_config != 0)
	{
		return 1;
	}
	//check if already configured

	LldSpiResetConfig(spi_object);
	//reset the configuration before we config as a precaution

	uint32_t ret = LldSpiConfigMasterInterrupt(spi_object, spi_config);
	//configure the spi to be used.

	spi_object->spi_config = spi_config;
	//set spi as configured.

	return ret;
}


//
// SPI TRANSMIT INTERRUPT 
//
uint32_t SpiTransmitInterrupt(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data)
{
return LldSpiTransmitInterrupt(spi_object,data_out,num_data);
}

//
// SPI TRANSFER INTERRUPT 
//
uint32_t SpiTransferInterrupt(
	struct SpiObject *spi_object, 
	void *data_out,
	void *data_in,
	uint32_t num_data)
{
return LldSpiTransferInterrupt(spi_object,data_out,data_in,num_data);
}



//
// SPI Receive INTERRUPT 
//
uint32_t SpiReceiveInterrupt(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	return LldSpiReceiveInterrupt(spi_object,data_in,num_data);
}



//
// SPI_CONFIG DMA
//
uint32_t SpiConfigMasterDma(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config)
{
	if(spi_object->spi_config != 0)
	{
		return 1;
	}
	//check if already configured

	LldSpiResetConfig(spi_object);
	//reset the configuration before we config as a precaution

	uint32_t ret = LldSpiConfigMasterDma(spi_object, spi_config);
	//configure the spi to be used.

	spi_object->spi_config = spi_config;
	//set spi as configured.

	return ret;
}


//
// SPI TRANSMIT DMA 
//
uint32_t SpiTransmitDma(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data)
{
return LldSpiTransmitDma(spi_object,data_out,num_data);
}

//
// SPI TRANSFER DMA 
//
uint32_t SpiTransferDma(
	struct SpiObject *spi_object, 
	void *data_out,
	void *data_in,
	uint32_t num_data)
{
return LldSpiTransferDma(spi_object,data_out,data_in,num_data);
}



//
// SPI Receive DMA 
//
uint32_t SpiReceiveDma(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	return LldSpiReceiveDma(spi_object,data_in,num_data);
}




