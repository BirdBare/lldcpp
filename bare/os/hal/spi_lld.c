//
//
//
//
//



#include "spi_lld.h"

// MUST BE DECLARED FOR HAL LIBRARY
uint32_t (*SPI_TRANSMIT_FUNCTIONS[3])(struct SpiLld *spi_lld) =
	{SpiTransmitPolled,SpiTransmitInterrupt,SpiTransmitDma}; 

uint32_t (*SPI_TRANSFER_FUNCTIONS[3])(struct SpiLld *spi_lld) =
	{SpiTransferPolled,SpiTransferInterrupt,SpiTransferDma}; 

uint32_t (*SPI_RECEIVE_FUNCTIONS[3])(struct SpiLld *spi_lld) =
	{0,0,0}; 
// END


uint32_t SpiConfig(
	struct SpiLld * const spi_lld,
	struct SpiConfig * const spi_config)
{
	uint32_t bus_speed = ClockGetSpeed(spi_lld->rcc_lld.peripheral_bus);
	//get bus speed because we will use it in comparison

	uint32_t br = 0 - 1;
	//counter starts at zero. so first count will overflow to zero

	do
	{
		bus_speed >>= 1;
		br++;
	} while(spi_config->clock_frequency < bus_speed && br < 7);
	//calculates actual clock speed and finds the correct register value

	spi_config->clock_frequency = bus_speed;
	//set actual spi speed for user

	spi_config->cr1 |= (br << 3);
	//set SPI_CR1 register values.

	spi_lld->spi_config = spi_config;
	//set pointer for spi actions

	return 0;
}

uint32_t SpiResetConfig(
	const struct SpiLld * const spi_lld)
{
	if((spi_lld->spi->CR1 & SPI_CR1_SPE) == 0)
	{
		RccResetPeripheral(&spi_lld->rcc_lld);

		return 0;
	}

	return 1;
}

//############# POLLED FLAGS SPI CONTROL ########################

//
// SPI TRANSMIT POLLED
//
uint32_t SpiTransmitPolled(
	struct SpiLld *spi_lld)
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_lld->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SPE |
		(crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	for(uint32_t counter = 0; counter < spi_config->num_data; counter++)
	{
		if(dff == 0)
		{
			spi->DR = ((uint8_t *)spi_config->data_out)[counter];
		}
		else
		{
			spi->DR = ((uint16_t *)spi_config->data_out)[counter];
		}
		//decide between 8 bit and 16 bit data;

		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_TXE) == 0);
		//wait till buffer is empty
	}
	//transmit the data

	if(crcpr != 0)
	{
		spi->CR1 |= SPI_CR1_CRCNEXT;
		//if crc is enabled then the crc is transfered after the last data.
	}

	return 0;
}

//
// SPI TRANSFER POLLED
//
uint32_t SpiTransferPolled(
	struct SpiLld *spi_lld) 
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi
	
	const struct SpiConfig *spi_config = spi_lld->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	const uint32_t num_data = spi_config->num_data;

	for(uint32_t counter = 0; counter < num_data; counter++)
	{
		if(dff == 0)
		{
			spi->DR = ((uint8_t *)spi_config->data_out)[counter];
		}
		else
		{
			spi->DR = ((uint16_t *)spi_config->data_out)[counter];
		}
		//decide between 8 bit and 16 bit data;
		
		if(counter == (num_data - 1) && crcpr != 0)
		{
			spi->CR1 |= SPI_CR1_CRCNEXT;
			//if crc is enabled then the crc is transfered after the last data.
		}

		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_RXNE) == 0);
		//wait till buffer is not empty

		if(dff == 0)
		{
			((uint8_t *)spi_config->data_in)[counter] = spi->DR;
		}
		else
		{
			((uint16_t *)spi_config->data_in)[counter] = spi->DR;
		}
		//decide between 8 bit and 16 bit data;
	}
	//transmit and receive the data

	return 0;
}



//
// SPI Receive POLLED DOES NOT CURRENTLY WORK DUE TO MASTER RECEIVE ONLY ERROR
//
uint32_t SpiReceivePolled(
	struct SpiLld *spi_lld) 
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_lld->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_RXONLY | SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	const uint32_t num_data = spi_config->num_data;

	for(uint32_t counter = 0; counter < num_data; counter++)
	{
		do
		{
			asm volatile("");
		} while((spi->SR & SPI_SR_RXNE) == 0);
		//wait till buffer is not empty

		if(dff == 0)
		{
			((uint8_t *)spi_config->data_in)[counter] = spi->DR;
		}
		else
		{
			((uint16_t *)spi_config->data_in)[counter] = spi->DR;
		}
		//decide between 8 bit and 16 bit data;

		if(counter == (num_data - 2) && crcpr != 0)
		{
			spi->CR1 |= SPI_CR1_CRCNEXT;
			//if crc is enabled then the crc is transfered after the second last data.
		}
	}
	//receive the data

	return 0;
}
//######################### END POLLED FLAGS SPI CONTROL ##################

//
// SPI TRANSFER DMA
//
uint32_t SpiTransmitDma(
	struct SpiLld *spi_lld) 
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_lld->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SPE |
		(crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	struct DmaLld * tx_dma_lld = spi_lld->tx_dma_lld;
	//get tx dma lld

	DmaClearFlags(tx_dma_lld,0b111101);

	DmaConfigNDTR(tx_dma_lld, spi_config->num_data);
	DmaConfigPAR(tx_dma_lld, (uint32_t *)&spi->DR);
	DmaConfigM0AR(tx_dma_lld, spi_config->data_out);
	DmaConfigCR(tx_dma_lld, (spi_lld->tx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | 1 << 6 | 1);

	spi->CR2 |= SPI_CR2_TXDMAEN;
	//enable dma request for transfer

	return 0;
}

uint32_t SpiTransferDma(
	struct SpiLld *spi_lld) 
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi
	
	const struct SpiConfig *spi_config = spi_lld->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->DR; //dummy read

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	struct DmaLld * tx_dma_lld = spi_lld->tx_dma_lld;
	struct DmaLld * rx_dma_lld = spi_lld->rx_dma_lld;
	//get tx & rx dma lld

	DmaClearFlags(tx_dma_lld,0b111101);
	DmaClearFlags(rx_dma_lld,0b111101);


	DmaConfigNDTR(tx_dma_lld, spi_config->num_data);
	DmaConfigNDTR(rx_dma_lld, spi_config->num_data);
	DmaConfigPAR(tx_dma_lld, (uint32_t *)&spi->DR);
	DmaConfigPAR(rx_dma_lld, (uint32_t *)&spi->DR);
	DmaConfigM0AR(tx_dma_lld, spi_config->data_out);
	DmaConfigM0AR(rx_dma_lld, spi_config->data_in);
	DmaConfigCR(tx_dma_lld, (spi_lld->tx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | 1 << 6 | 1);
	DmaConfigCR(rx_dma_lld, (spi_lld->rx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | 1);

	spi->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;
	//enable dma request for transfer

	return 0;
}

// SPI Receive DOES NOT CURRENTLY WORK DUE TO MASTER RECEIVE ONLY ERROR
uint32_t SpiReceiveDma(
	struct SpiLld *spi_lld) 
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi
	
	const struct SpiConfig *spi_config = spi_lld->spi_config;
	//get spi config

	uint32_t dff = spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer and get partial data size..

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->DR; //dummy read to reset RX flag. prevents overrun error.

	const uint32_t crcpr = spi->CRCPR = spi_config->crc_polynomial;
	//get crcpr for possible crc enable

	spi->CR1 |= SPI_CR1_RXONLY | SPI_CR1_SPE | (crcpr != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	dff &= SPI_CR1_DFF;
	//get data size

	struct DmaLld * rx_dma_lld = spi_lld->rx_dma_lld;
	//get rx dma lld

	DmaClearFlags(rx_dma_lld,0b111101);
	//clear all interrupt flags 

	DmaConfigNDTR(rx_dma_lld, spi_config->num_data);
	DmaConfigPAR(rx_dma_lld, (uint32_t *)&spi->DR);
	DmaConfigM0AR(rx_dma_lld, spi_config->data_in);
	DmaConfigCR(rx_dma_lld, (spi_lld->rx_dma_channel << 25) | DMA_SxCR_MINC |
		dff << 2 | dff | DMA_SxCR_EN);
	//set dma settings and enable dma for spi

	spi->CR2 |= SPI_CR2_RXDMAEN;
	//enable dma request for transfer. Transfer starts here.

	return 0;
}
//######################### END DMA SPI CONTROL ##################

//
//	SPI TRANSMIT INTERRUPTS
//
uint32_t SpiTransmitInterrupt(
	struct SpiLld *spi_lld) 
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_lld->spi_config;
	//get spi config

	spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer 

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SPE |
		((spi->CRCPR = spi_config->crc_polynomial) != 0 ? SPI_CR1_CRCEN : 0);
	//enable 1 way mode. always enabled so we dont get overrun error. 
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	spi->CR2 |= SPI_CR2_TXEIE;
	//enable dma request for transfer. starts the transfer

	return 0;
}


//
//  SPI TRANSFER INTERRUPT
//
uint32_t SpiTransferInterrupt(
	struct SpiLld *spi_lld) 
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi

	const struct SpiConfig *spi_config = spi_lld->spi_config;
	//get spi config

	spi->CR1 = spi_config->cr1;
	//reset spi settings for new transfer 

	spi->CR2 = spi_config->cr2;
	//reset cr2 register to user settings

	spi->DR; //dummy read to reset RX flag. prevents error.

	spi->CR1 |=  SPI_CR1_SPE | 
		((spi->CRCPR = spi_config->crc_polynomial) != 0 ? SPI_CR1_CRCEN : 0);
	//try to enable crc if crc polynomial is set.
	//enable spi too.

	spi->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;
	//enable dma request for transfer. starts the transfer

	return 0;
}
//####################### END INTERRUPT CONTROL ##################



//
// SPI GENERAL INTERRUPT FUNCTION GET DATA FOR OUTPUT 
//
uint32_t SPI_INTERRUPT_GET(struct SpiLld *spi_lld, uint32_t *data)
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi

	struct SpiConfig *spi_config = spi_lld->spi_config;

	uint32_t dff = spi_config->cr1 & SPI_CR1_DFF;

	static uint32_t tx_counter = 0;
	//counter for the spi data when getting it from array

		if(dff == 0)
		{
			*data = ((uint8_t *)spi_config->data_out)[tx_counter++];
		}
		else
		{
			*data = ((uint16_t *)spi_config->data_out)[tx_counter++];
		}
		//decide between 8 bit and 16 bit data;
	
		if(tx_counter == spi_config->num_data)
		{
			if(spi->CRCPR != 0)
			{
				spi->CR1 |= SPI_CR1_CRCNEXT;
				//if crc is enabled then the crc is transfered after the last data.
			}
		
			spi->CR2 &= ~SPI_CR2_TXEIE;
			//if buffer is empty then disable interrupt

			tx_counter = 0;
			//reset static counter to zero for next transmission

			return 1;
		}

	return 0;
}

//
// SPI INTERRUPT FUNCTION STORE RECEIVED DATA
//
uint32_t SPI_INTERRUPT_PUT(struct SpiLld *spi_lld, uint32_t *data)
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi

	struct SpiConfig *spi_config = spi_lld->spi_config;

	uint32_t dff = spi_config->cr1 & SPI_CR1_DFF;

	static uint32_t rx_counter = 0;
	//counter for the spi data when getting it from array

		if(dff == 0)
		{
			((uint8_t *)spi_config->data_in)[rx_counter++] = *data;
		}
		else
		{
			((uint16_t *)spi_config->data_in)[rx_counter++] = *data;
		}
		//decide between 8 bit and 16 bit data;

		if(rx_counter == spi_config->num_data)
		{
			spi->CR2 &= ~SPI_CR2_RXNEIE;
			//if buffer is empty then disable interrupt

			rx_counter = 0;

			return 1;
		}

	return 0;
}


//
// SPI GENERAL INTERRUPT HANDLER
//
ALWAYS_INLINE void GENERAL_SPI_HANDLER(struct SpiLld *spi_lld)
{
	volatile SPI_TypeDef *spi = spi_lld->spi;
	//get spi

	if((spi->CR2 & SPI_CR2_RXNEIE) != 0 && (spi->SR & SPI_SR_RXNE) != 0)
	{
		SPI_INTERRUPT_PUT(spi_lld, (uint32_t *)&spi->DR);	
	}
//DEAL WITH RX

	if((spi->CR2 & SPI_CR2_TXEIE) != 0 && (spi->SR & SPI_SR_TXE) != 0)
	{
		SPI_INTERRUPT_GET(spi_lld, (uint32_t *)&spi->DR);	
	}
//DEAL WITH TX

	
if((spi->SR & SPI_SR_UDR) != 0)
 BREAK(1);
if((spi->SR & SPI_SR_CRCERR) != 0)
 BREAK(2);
if((spi->SR & SPI_SR_MODF) != 0)
 BREAK(3);
if((spi->SR & SPI_SR_OVR) != 0)
 BREAK(4);
if((spi->SR & SPI_SR_FRE) != 0)
 BREAK(5);
//DEAL WITH FLAGS
}

#define SPI_HANDLER(spi_number) \
void SPI1_IRQHandler(void) \
{	\
	void (*interrupt)(struct SpiLld *spi_lld) = \
		SPI1_LLD.spi_config->interrupt; \
	/*get user set interrupt address*/ \
 \
	if(interrupt == 0) \
	{ \
		GENERAL_SPI_HANDLER(&SPI1_LLD); \
		/*if interrupt is not set then we run the general interrupt*/ \
	} \
	else \
	{ \
		interrupt(&SPI1_LLD); \
		/*if set then we run user interrupt instead*/ \
	} \
	/*if it is set then we always run it instead of the default */ \
} \


#ifdef SPI1
struct SpiLld SPI1_LLD ={{0x44,12,3},
SPI1_TX_DMA_CHANNEL,SPI1_RX_DMA_CHANNEL,
SPI1_TX_DMA_LLD,SPI1_RX_DMA_LLD,
SPI1};

void SPI1_IRQHandler(void)
{	
	void (*interrupt)(struct SpiLld *spi_lld) =
		SPI1_LLD.spi_config->interrupt;
	//get user set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI1_LLD);
		//if interrupt is not set then we run the general interrupt
	}
	else
	{
		interrupt(&SPI1_LLD);
		//if set then we run user interrupt instead
	}
	//if it is set then we always run it instead of the default

}
#endif

#ifdef SPI2
struct SpiLld SPI2_LLD = {{0x40,14,2},
SPI2_TX_DMA_CHANNEL,SPI2_RX_DMA_CHANNEL,
SPI2_TX_DMA_LLD,SPI2_RX_DMA_LLD,
SPI2};

void SPI2_IRQHandler(void)
{	
	void (*interrupt)(struct SpiLld *spi_lld) =
		SPI2_LLD.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI2_LLD);
	}
	else
	{
		interrupt(&SPI2_LLD);
	}
	//if it is set then we always run it instead of the default

}

#endif

#ifdef SPI3
struct SpiLld SPI3_LLD = {{0x40,15,2},
SPI3_TX_DMA_CHANNEL,SPI3_RX_DMA_CHANNEL,
SPI3_TX_DMA_LLD,SPI3_RX_DMA_LLD,
SPI3};

void SPI3_IRQHandler(void)
{	
	void (*interrupt)(struct SpiLld *spi_lld) =
		SPI3_LLD.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI3_LLD);
	}
	else
	{
		interrupt(&SPI3_LLD);
	}
	//if it is set then we always run it instead of the default

}

#endif

#ifdef SPI4
struct SpiLld SPI4_LLD = {{0x44,13,3},
SPI4_TX_DMA_CHANNEL,SPI4_RX_DMA_CHANNEL,
SPI4_TX_DMA_LLD,SPI4_RX_DMA_LLD,
SPI4};

void SPI4_IRQHandler(void)
{	
	void (*interrupt)(struct SpiLld *spi_lld) =
		SPI4_LLD.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI4_LLD);
	}
	else
	{
		interrupt(&SPI4_LLD);
	}
	//if it is set then we always run it instead of the default

}

#endif

#ifdef SPI5
struct SpiLld SPI5_LLD = {{0x44,20,3},
SPI5_TX_DMA_CHANNEL,SPI5_RX_DMA_CHANNEL,
SPI5_TX_DMA_LLD,SPI5_RX_DMA_LLD,
SPI5};

void SPI5_IRQHandler(void)
{	
	void (*interrupt)(struct SpiLld *spi_lld) =
		SPI5_LLD.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI5_LLD);
	}
	else
	{
		interrupt(&SPI5_LLD);
	}
	//if it is set then we always run it instead of the default

}

#endif

#ifdef SPI6
struct SpiLld SPI6_LLD = {{0x44,21,3},
SPI6_TX_DMA_CHANNEL,SPI6_RX_DMA_CHANNEL,
SPI6_TX_DMA_LLD,SPI6_RX_DMA_LLD,
SPI6};

void SPI6_IRQHandler(void)
{	
	void (*interrupt)(struct SpiLld *spi_lld) =
		SPI6_LLD.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI6_LLD);
	}
	else
	{
		interrupt(&SPI6_LLD);
	}
	//if it is set then we always run it instead of the default

}

#endif





















