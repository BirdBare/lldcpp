//
//
//
//
//



#include "spi_hal.h"


//
// SPI_CONFIG MASTER 
//
uint32_t SpiConfigMaster(
	struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config)
{
	BareOSDisableInterrupts();

	if(spi_object->spi_config != 0)
	{
		return 1;
	}
	//check if already configured

	LldSpiResetConfig(spi_object);
	//reset the configuration before we config as a precaution

	uint32_t ret = LldSpiConfigMaster(spi_object, spi_config);
	//configure the spi to be used.

	spi_object->spi_config = spi_config;
	//set spi as configured.

	BareOSEnableInterrupts();

	return ret;
}

//
// SPI RESET CONFIG
//
uint32_t SpiResetConfig(
	struct SpiObject * const spi_object)
{
	BareOSDisableInterrupts();

	uint32_t ret  = LldSpiResetConfig(spi_object);

	BareOSEnableInterrupts();
	
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
// SPI TRANSMIT INTERRUPT 
//
uint32_t SpiTransmitInterrupt(
	struct SpiObject *spi_object,
	void *data_out,
	uint32_t num_data)
{
	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

	BareOSDisableInterrupts();
	uint32_t ret = LldSpiTransmitInterrupt(spi_object,data_out,num_data);
	BareOSEnableInterrupts();
	//start transmission atomically	

	BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
	BareOSCallSwitch();
	//switch out of dead thread

	spi_object->spi_config->callback = 0;
	//reset callback

	return ret;
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
	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

  BareOSDisableInterrupts();
	uint32_t ret = LldSpiTransferInterrupt(spi_object,data_out,data_in,num_data);
	BareOSEnableInterrupts();
	//start transmission atomically	

	BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
	BareOSCallSwitch();
	//switch out of dead thread

	spi_object->spi_config->callback = 0;
	//reset callback

	return ret;
}



//
// SPI Receive INTERRUPT 
//
uint32_t SpiReceiveInterrupt(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

  BareOSDisableInterrupts();
	uint32_t ret = LldSpiReceiveInterrupt(spi_object,data_in,num_data);
	BareOSEnableInterrupts();
	//start transmission atomically	

	BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
	BareOSCallSwitch();
	//switch out of dead thread

	spi_object->spi_config->callback = 0;
	//reset callback

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
	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

  BareOSDisableInterrupts();
	uint32_t ret = LldSpiTransmitDma(spi_object,data_out,num_data);
	BareOSEnableInterrupts();
	//start transmission atomically	

	BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
	BareOSCallSwitch();
	//switch out of dead thread

	spi_object->spi_config->callback = 0;
	//reset callback

	return ret;
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
	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

  BareOSDisableInterrupts();
	uint32_t ret = LldSpiTransferDma(spi_object,data_out,data_in,num_data);
	BareOSEnableInterrupts();
	//start transmission atomically	

	BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
	BareOSCallSwitch();
	//switch out of dead thread

	spi_object->spi_config->callback = 0;
	//reset callback

	return ret;
}



//
// SPI Receive DMA 
//
uint32_t SpiReceiveDma(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

  BareOSDisableInterrupts();
	uint32_t ret = LldSpiReceiveDma(spi_object,data_in,num_data);
	BareOSEnableInterrupts();
	//start transmission atomically	

	BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
	BareOSCallSwitch();
	//switch out of dead thread

	spi_object->spi_config->callback = 0;
	//reset callback

	return ret;
}




