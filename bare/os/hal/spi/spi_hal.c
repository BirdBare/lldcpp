//
//
//
//
//



#include "spi_hal.h"


//
// SPI_CONFIG
//
uint32_t SpiConfig(
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

	uint32_t ret = LldSpiConfig(spi_object, spi_config);
	//configure the spi to be used.

	spi_object->spi_config = spi_config;
	//set spi as configured.

	return ret;
}

//
// SPI RESET CONFIG
//
uint32_t SpiResetConfig(
	struct SpiObject * const spi_object)
{
	return LldSpiResetConfig(spi_object);
}

//############# POLLED FLAGS SPI CONTROL ########################

//
// SPI TRANSMIT 
//
uint32_t SpiTransmit(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data)
{
return LldSpiTransmit(spi_object,data_out,num_data);
}

//
// SPI TRANSFER 
//
uint32_t SpiTransfer(
	struct SpiObject *spi_object, 
	void *data_out,
	void *data_in,
	uint32_t num_data)
{
return LldSpiTransfer(spi_object,data_out,data_in,num_data);
}



//
// SPI Receive 
//
uint32_t SpiReceive(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	return LldSpiReceive(spi_object,data_in,num_data);
}
//######################### END POLLED FLAGS SPI CONTROL ##################




