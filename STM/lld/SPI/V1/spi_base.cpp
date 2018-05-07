//
//
//
//
//
//



#include "spi_base.hpp"

#include "spi_object.cpp"


#ifdef SPI1
struct SpiHal SPI1_HAL ={
	{&RCC->APB2ENR,
	RCC_APB2ENR_SPI1EN,
	&RCC->APB2RSTR,
	RCC_PERIPHERAL_BUS_APB2},
	{1,
	(IRQn_Type[1]){SPI1_IRQn}},
	2, //tx num
	2, //rx num
	(uint8_t [2]){3,3}, //tx channels
	(uint8_t [2]){3,3}, //rx channels
	(DmaHal *[2]){&DMA2S3_HAL,&DMA2S5_HAL}, //tx dmas
	(DmaHal *[2]){&DMA2S0_HAL,&DMA2S2_HAL}, //rx dmas
	SPI1};

extern "C"
{
void SPI1_IRQHandler(void);
}

void SPI1_IRQHandler(void)
{	
	SpiBase *spi_object = SPI1_HAL.owner;

	if(spi_object == 0 || spi_object->Interrupt() == 0)
		BREAK(0);

	spi_object->Interrupt()(spi_object->InterruptArgs());
}
#endif

/*
#ifdef SPI2
struct SpiHal SPI2_HAL ={
	{0x40,
	14,
	APB1},
	{1,
	(IRQn_Type[1]){SPI2_IRQn}},
	SPI2_TX_DMA_CHANNEL,
	SPI2_RX_DMA_CHANNEL,
	SPI2_TX_DMA_HAL,
	SPI2_RX_DMA_HAL,
	SPI2};

void SPI2_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI2_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI2_HAL);
	}
	else
	{
		interrupt(SPI2_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI3
struct SpiHal SPI3_HAL ={
	{0x40,
	15,
	APB1},
	{1,
	(IRQn_Type[1]){SPI3_IRQn}},
	SPI3_TX_DMA_CHANNEL,
	SPI3_RX_DMA_CHANNEL,
	SPI3_TX_DMA_HAL,
	SPI3_RX_DMA_HAL,
	SPI3};

void SPI3_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI3_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI3_HAL);
	}
	else
	{
		interrupt(SPI3_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI4
struct SpiHal SPI4_HAL ={
	.rcc.reg_offset = 0x44,
	.rcc.bit_offset = 13,
	.rcc.nvic.num_irq = 1,
	.rcc.nvic.irq_number = (uint8_t[1]){51},
	.rcc.peripheral_bus = APB2,
	.tx_dma_channel = SPI4_TX_DMA_CHANNEL,
	.rx_dma_channel = SPI4_RX_DMA_CHANNEL,
	.tx_dma_object = SPI4_TX_DMA_HAL,
	.rx_dma_object =SPI4_RX_DMA_HAL,
	.spi = SPI4};

void SPI4_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI4_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI4_HAL);
	}
	else
	{
		interrupt(SPI4_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI5
struct SpiHal SPI5_HAL ={
	.rcc.reg_offset = 0x44,
	.rcc.bit_offset = 20,
	.rcc.nvic.num_irq = 1,
	.rcc.nvic.irq_number = (uint8_t[1]){51},
	.rcc.peripheral_bus = APB2,
	.tx_dma_channel = SPI5_TX_DMA_CHANNEL,
	.rx_dma_channel = SPI5_RX_DMA_CHANNEL,
	.tx_dma_object = SPI5_TX_DMA_HAL,
	.rx_dma_object =SPI5_RX_DMA_HAL,
	.spi = SPI5};

void SPI5_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI5_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI5_HAL);
	}
	else
	{
		interrupt(SPI5_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

#ifdef SPI6
struct SpiHal SPI6_HAL ={
	.rcc.reg_offset = 0x44,
	.rcc.bit_offset = 21,
	.rcc.nvic.num_irq = 1,
	.rcc.nvic.irq_number = (uint8_t[1]){51},
	.rcc.peripheral_bus = APB2,
	.tx_dma_channel = SPI6_TX_DMA_CHANNEL,
	.rx_dma_channel = SPI6_RX_DMA_CHANNEL,
	.tx_dma_object = SPI6_TX_DMA_HAL,
	.rx_dma_object =SPI6_RX_DMA_HAL,
	.spi = SPI6};

void SPI6_IRQHandler(void)
{	
	void (*interrupt)(void *args) =
		SPI6_HAL.spi_config->interrupt;
	//get use set interrupt address

	if(interrupt == 0)
	{
		GENERAL_SPI_HANDLER(&SPI6_HAL);
	}
	else
	{
		interrupt(SPI6_HAL.spi_config->interrupt_args);
	}
	//if it is set then we always run it instead of the default
}
#endif

*/




















