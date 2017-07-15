//
//
//
//
//



#include "spi_lld.h"

const uint8_t cf_divider[8] = {2,4,8,16,32,64,128,255};

#ifdef SPI1
struct SpiObject SPI1_OBJECT = {{0x44,12,3},0,SPI1};
#endif

#ifdef SPI2
struct SpiObject SPI2_OBJECT = {{0x40,14,2},0,SPI2};
#endif

#ifdef SPI3
struct SpiObject SPI3_OBJECT = {{0x40,15,2},0,SPI3};
#endif

#ifdef SPI4
struct SpiObject SPI4_OBJECT = {{0x44,13,3},0,SPI4};
#endif

#ifdef SPI5
struct SpiObject SPI5_OBJECT = {{0x44,20,3},0,SPI5};
#endif

#ifdef SPI6
struct SpiObject SPI6_OBJECT = {{0x44,21,3},0,SPI6};
#endif


uint32_t SpiConfig(
	const struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config)
{
	volatile SPI_TypeDef * const spi = spi_object->spi;
	//get spi peripheral

	if((spi->CR1 & SPI_CR1_SPE) != 0) //if SPE is set then spi is already in use
	{
		return SPICONFIG_ENABLED;
		//spi already in use so return error.
	}

	const uint32_t clock_freq_divider = 
		ClockGetPeripheralSpeed(&spi_object->rcc) / spi_config->clock_frequency;
	//get Peripheral specific clock speed and divide by desired clock freq to get
	//divider

	uint32_t cf_index = 0;
	//start clock freq divider at index zero

	while(cf_divider[cf_index] < clock_freq_divider && cf_index != 7)
	//seven is the end of the array so we must stop at seven or when the divider
	//is higher.
	{
		++cf_index;
	}
	//get br value for CR1 register. Always higher than needed so clock speed is
	//never too high	

	spi_config->cr1 |= (cf_index << 3) | SPI_CR1_SPE;
	//set BR and SPE in spi config struct before setting spi registers

	spi->CRCPR = spi_config->crcpr;
	spi->CR2 = spi_config->cr2;
	spi->CR1 = spi_config->cr1;
	//set spi registers

	return 0;
}


uint32_t SpiResetConfig(
	const struct SpiObject * const spi_object)
{
	if((spi_object->spi->CR1 & SPI_CR1_SPE) == 0)
	{
		RccResetPeripheral(&spi_object->rcc);

		return 0;
	}

	return SPICONFIG_ENABLED;
}



uint32_t SpiDisable(
	const struct SpiObject * const spi_object)
{
	volatile SPI_TypeDef * const spi = spi_object->spi;

	uint32_t status_reg = spi->SR;

	if((status_reg & SPI_SR_BSY) == 0 && 
		(status_reg & (SPI_SR_RXNE | SPI_SR_TXE)) == (SPI_SR_RXNE | SPI_SR_TXE))
	{	
		spi->CR1 &= ~SPI_CR1_SPE;

		return 0;
	}
	return SPI_DISABLE_TRANSFER;
}

































#define SPI_DATA_TIMEOUT 3

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t SpiPut8Blocking(
	const struct SpiObject * const spi_object,
	const struct CommunicationConfig * const communication_config)
{
	const volatile SPI_TypeDef * spi = spi_object->spi;

	const uint32_t milli_ref = SysTickGetMilli();

	const uint32_t timeout_milli = communication_config->timeout_milli;
	uint32_t num_data = communication_config->num_data;
	uint8_t *tx_data = communication_config->tx_data;

	do
	{
		while((spi->SR & SPI_SR_TXE) == 0)
		{
			if((SysTickGetMilli() - milli_ref) > timeout_milli)
			{
				return SPI_DATA_TIMEOUT;
			}
			//Checks if we have timed out
		}
		//waits for space in the TX register

		ASM(" strb %1, [%0, #0xC]" ::"r" (spi), "r" (*(tx_data++)));
		//put data in data register

	} while(--num_data != 0);
	//run until no more data is left

	return 0;
}

uint32_t SpiPut16Blocking(
	const struct SpiObject * const spi_object,
	const struct CommunicationConfig * const communication_config)
{
	const volatile SPI_TypeDef * spi = spi_object->spi;

	const uint32_t milli_ref = SysTickGetMilli();

	const uint32_t timeout_milli = communication_config->timeout_milli;
	uint32_t num_data = communication_config->num_data;
	uint16_t *tx_data = communication_config->tx_data;

	do
	{
		while((spi->SR & SPI_SR_TXE) == 0)
		{
			if((SysTickGetMilli() - milli_ref) > timeout_milli)
			{
				return SPI_DATA_TIMEOUT;
			}
			//Checks if we have timed out
		}
		//waits for space in the TX register

		ASM(" strh %1, [%0, #0xC]" ::"r" (spi), "r" (*(tx_data++)));
		//put data in data register

	} while(--num_data != 0);
	//run until no more data is left

	return 0;
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t SpiGet8Blocking(
	const struct SpiObject * const spi_object,
	const struct CommunicationConfig * const communication_config)
{
	const volatile SPI_TypeDef * spi = spi_object->spi;

	const uint32_t milli_ref = SysTickGetMilli();

	const uint32_t timeout_milli = communication_config->timeout_milli;
	uint32_t num_data = communication_config->num_data;
	uint8_t *rx_data = communication_config->rx_data;


	do
	{
		while((spi->SR & SPI_SR_RXNE) == 0)
		{
			if((SysTickGetMilli() - milli_ref) > timeout_milli)
			{
				return SPI_DATA_TIMEOUT;
			}
			//Checks if we have timed out
		}
		//waits for space in the RX register

		ASM(" ldrb %0, [%1, #0xC]" :"=r" (*(rx_data++)) : "r" (spi));
		//get data from data register

	} while(--num_data != 0);
	//run until no more data is left

	return 0;
}

uint32_t SpiGet16Blocking(
	const struct SpiObject * const spi_object,
	const struct CommunicationConfig * const communication_config)
{
	const volatile SPI_TypeDef * spi = spi_object->spi;

	const uint32_t milli_ref = SysTickGetMilli();

	const uint32_t timeout_milli = communication_config->timeout_milli;
	uint32_t num_data = communication_config->num_data;
	uint16_t *rx_data = communication_config->rx_data;

	do
	{
		while((spi->SR & SPI_SR_RXNE) == 0)
		{
			if((SysTickGetMilli() - milli_ref) > timeout_milli)
			{
				return SPI_DATA_TIMEOUT;
			}
			//Checks if we have timed out
		}
		//waits for space in the RX register

		ASM(" ldrh %0, [%1, #0xC]" :"=r" (*(rx_data++)) : "r" (spi));
		//get data from data register

	} while(--num_data != 0);
	//run until no more data is left

	return 0;
}






