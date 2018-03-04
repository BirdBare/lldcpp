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
	MutexLock(&spi_object->mutex);

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

	MutexUnlock(&spi_object->mutex);

	return ret;
}

//
// SPI RESET CONFIG
//
uint32_t SpiResetConfig(
	struct SpiObject * const spi_object)
{
	MutexLock(&spi_object->mutex);

	uint32_t ret  = LldSpiResetConfig(spi_object);

	MutexUnlock(&spi_object->mutex);
	
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
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

	MutexLock(&spi_object->mutex);

	uint32_t ret = LldSpiTransmitPolled(spi_object,data_out,num_data);

	MutexUnlock(&spi_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}


	return ret;
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
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

	MutexLock(&spi_object->mutex);

	uint32_t ret = LldSpiTransferPolled(spi_object,data_out,data_in,num_data);

	MutexUnlock(&spi_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}


	return ret;
}



//
// SPI Receive POLLED 
//
uint32_t SpiReceivePolled(
	struct SpiObject *spi_object, 
	void *data_in,
	uint32_t num_data)
{
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

	MutexLock(&spi_object->mutex);

	uint32_t ret = LldSpiReceivePolled(spi_object,data_in,num_data);

	MutexUnlock(&spi_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}



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
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

	MutexLock(&spi_object->mutex);

	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

	uint32_t ret = LldSpiTransmitInterrupt(spi_object,data_out,num_data);
	//start transmission atomically	

	if(ret == 0)
	{
		BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
		BareOSCallSwitch();
	//switch out of dead thread
	}

	spi_object->spi_config->callback = 0;
	//reset callback

	MutexUnlock(&spi_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}


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
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

	MutexLock(&spi_object->mutex);

	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

	uint32_t ret = LldSpiTransferInterrupt(spi_object,data_out,data_in,num_data);
	//start transmission atomically	

	if(ret == 0)
	{
		BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
		BareOSCallSwitch();
	//switch out of dead thread
	}

	spi_object->spi_config->callback = 0;
	//reset callback

	MutexUnlock(&spi_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}


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
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

	MutexLock(&spi_object->mutex);

	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

	uint32_t ret = LldSpiReceiveInterrupt(spi_object,data_in,num_data);
	//start transmission atomically	

	if(ret == 0)
	{
		BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
		BareOSCallSwitch();
	//switch out of dead thread
	}

	spi_object->spi_config->callback = 0;
	//reset callback

	MutexUnlock(&spi_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

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
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

//	MutexLock(&spi_object->tx_dma_object->mutex);

	MutexLock(&spi_object->mutex);

	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

	uint32_t ret = LldSpiTransmitDma(spi_object,data_out,num_data);
	//start transmission atomically	

	if(ret == 0)
	{
		BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
		BareOSCallSwitch();
	//switch out of dead thread
	}

	spi_object->spi_config->callback = 0;
	//reset callback

	MutexUnlock(&spi_object->mutex);

//	MutexUnlock(&spi_object->tx_dma_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}


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
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

//	MutexLock(&spi_object->rx_dma_object->mutex);
//	MutexLock(&spi_object->tx_dma_object->mutex);

	MutexLock(&spi_object->mutex);

	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

	uint32_t ret = LldSpiTransferDma(spi_object,data_out,data_in,num_data);
	//start transmission atomically	

	if(ret == 0)
	{
		BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
		BareOSCallSwitch();
	//switch out of dead thread
	}

	spi_object->spi_config->callback = 0;
	//reset callback

	MutexUnlock(&spi_object->mutex);

//	MutexUnlock(&spi_object->rx_dma_object->mutex);
//	MutexUnlock(&spi_object->tx_dma_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

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
	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexLock(&spi_object->spi_config->slave_gpio_object->mutex);
	}

//	MutexLock(&spi_object->rx_dma_object->mutex);

	MutexLock(&spi_object->mutex);

	spi_object->spi_config->callback = (void *)&BareOSSchedulerAddThread;
	spi_object->spi_config->callback_args = BareOSSchedulerGetCurrentThread();
	//set callback to ready thread once transmission is finished

	uint32_t ret = LldSpiReceiveDma(spi_object,data_in,num_data);
	//start transmission atomically	

	if(ret == 0)
	{
		BareOSSchedulerRemoveThread(BareOSSchedulerGetCurrentThread());
		BareOSCallSwitch();
	//switch out of dead thread
	}

	spi_object->spi_config->callback = 0;
	//reset callback

	MutexUnlock(&spi_object->mutex);

//	MutexUnlock(&spi_object->rx_dma_object->mutex);

	if(spi_object->spi_config->slave_gpio_object != 0 && 
		spi_object->spi_config->interrupt != 0)
	{
		MutexUnlock(&spi_object->spi_config->slave_gpio_object->mutex);
	}


	return ret;
}




