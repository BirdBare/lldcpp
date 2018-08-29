//
//
//
//
//



#include "dma_lld.hpp"

#ifdef DMA1
struct DmaHal
	DMA1S0_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA1EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream0_IRQn}},
		0,
		0,
		DMA1_Stream0};

struct DmaHal
DMA1S1_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA1EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream1_IRQn}},
		0,
		6,
		DMA1_Stream1};

struct DmaHal
DMA1S2_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA1EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream2_IRQn}},
		0,
		16,
		DMA1_Stream2};

struct DmaHal
DMA1S3_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA1EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream3_IRQn}},
		0,
		22,
		DMA1_Stream3};

struct DmaHal
DMA1S4_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA1EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream4_IRQn}},
		4,
		0,
		DMA1_Stream4};

struct DmaHal
DMA1S5_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA1EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream5_IRQn}},
		4,
		6,
		DMA1_Stream5};

struct DmaHal
DMA1S6_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA1EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream6_IRQn}},
		4,
		16,
		DMA1_Stream6};

struct DmaHal
DMA1S7_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA1EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream7_IRQn}},
		4,
		22,
		DMA1_Stream7};
#endif

#ifdef DMA2
struct DmaHal
DMA2S0_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA2EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream0_IRQn}},
		0,
		0,
		DMA2_Stream0};

struct DmaHal
DMA2S1_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA2EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream1_IRQn}},
		0,
		6,
		DMA2_Stream1};

struct DmaHal
DMA2S2_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA2EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream2_IRQn}},
		0,
		16,
		DMA2_Stream2};

struct DmaHal
DMA2S3_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA2EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream3_IRQn}},
		0,
		22,
		DMA2_Stream3};

struct DmaHal
DMA2S4_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA2EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream4_IRQn}},
		4,
		0,
		DMA2_Stream4};

struct DmaHal
DMA2S5_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA2EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream5_IRQn}},
		4,
		6,
		DMA2_Stream5};

struct DmaHal
DMA2S6_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA2EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream6_IRQn}},
		4,
		16,
		DMA2_Stream6};

struct DmaHal
DMA2S7_HAL = {
		{&RCC->AHB1ENR,
		RCC_AHB1ENR_DMA2EN,
		&RCC->AHB1RSTR,
		RCC_PERIPHERAL_BUS_AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream7_IRQn}},
		4,
		22,
		DMA2_Stream7};
#endif






//******************************************************************************
//
//
//
//******************************************************************************
void DmaObject::PreTransmission(void *par, void *m0ar, uint32_t length)
{
	//if(_hal.owner != this)
		BREAK(0);

	LldDmaClearFlags(_hal, 0b111101);
	//clear flags first

	_hal.dma->NDTR = length;
	//set num data to send

	_hal.dma->PAR = (uint32_t)par;
	//peripher address or memory area to send(tx memory)

	_hal.dma->M0AR = (uint32_t)m0ar;
	//should be memory area to receive or send the data

}


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaInterrupt::MemCopy(void *from, void *to, uint32_t length)
{
	_hal.dma->FCR = DMA_SxFCR_FEIE | DMA_SxFCR_DMDIS |
		DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1;
	//set error interrupt

	PreTransmission(from,to,length);

	_hal.dma->CR = _settings.cr | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | 
		_settings.data_size << 11 |	_settings.data_size << 13 | DMA_SxCR_MINC | 
		DMA_SxCR_PINC | DMA_SxCR_DIR_1 | DMA_SxCR_MBURST_0 | DMA_SxCR_PBURST_0 |
		CheckInterrupt() | DMA_SxCR_EN; 
	//config the addresses first return value is used in cr register
	//set error interrupts and other needed stuff

	return 0;
}



//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaInterrupt::MemSet(void *address, void *value, uint32_t length)
{
	_hal.dma->FCR = DMA_SxFCR_FEIE | DMA_SxFCR_DMDIS |
		DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1;
	//set error interrupt

	PreTransmission(value,address,length);

	_hal.dma->CR = _settings.cr | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | 
		_settings.data_size << 11 | _settings.data_size << 13 | DMA_SxCR_MINC | 
		DMA_SxCR_DIR_1 | DMA_SxCR_MBURST_0 | DMA_SxCR_PBURST_0 | DMA_SxCR_EN |
		CheckInterrupt();

	//config the addresses first return value is used in cr register
	//set error interrupts and other needed stuff


	return 0;
}



//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaInterrupt::TransferP2M(void *from, void *to, uint32_t length)
{
	_hal.dma->FCR = DMA_SxFCR_FEIE;
	//set error interrupt

	PreTransmission(from,to,length);

	_hal.dma->CR = _settings.cr | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | 
		_settings.data_size << 11 |  _settings.data_size << 13 | DMA_SxCR_MINC | 
		DMA_SxCR_EN | CheckInterrupt();
	//set error interrupts and other needed stuff 

	return 0;
} 


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaInterrupt::TransferM2P(void *from, void *to, uint32_t length)
{
	_hal.dma->FCR = DMA_SxFCR_FEIE;
	//set error interrupt

	PreTransmission(to,from,length);

	_hal.dma->CR = _settings.cr | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | 
		_settings.data_size << 11 | _settings.data_size << 13 | DMA_SxCR_MINC | 
		DMA_SxCR_DIR_0 | DMA_SxCR_EN | CheckInterrupt();
	//set error interrupts and other needed stuff 

	return 0;
} 
//transfer mem2mem, periph2mem, and mem2periph













//******************************************************************************
//	
//									DMA IRQ HANDLERS	 
//	
//******************************************************************************

static inline void DMA_STREAM_HANDLER(struct DmaHal &dma_object)
{
	//if(dma_object.owner == 0)
		BREAK(0);

	uint32_t flags = LldDmaGetFlags(dma_object);

	if((flags & 0b1101) != 0)
		BREAK(0);

	//((DmaInterrupt *)dma_object.owner)->GetCallback()(
		//((DmaInterrupt *)dma_object.owner)->GetCallbackArgs());
	//call callback

	LldDmaClearFlags(dma_object, flags);

	return;
}

#ifdef DMA1
extern "C"
{
void DMA1_Stream0_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA1S0_HAL);
}
void DMA1_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA1S1_HAL);
}

void DMA1_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA1S2_HAL);
}

void DMA1_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA1S3_HAL);
}

void DMA1_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA1S4_HAL);
}

void DMA1_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA1S5_HAL);
}

void DMA1_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA1S6_HAL);
}

void DMA1_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA1S7_HAL);
}
}
#endif

#ifdef DMA2
extern "C"
{
void DMA2_Stream0_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA2S0_HAL);
}

void DMA2_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA2S1_HAL);
}

void DMA2_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA2S2_HAL);
}

void DMA2_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA2S3_HAL);
}

void DMA2_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA2S4_HAL);
}

void DMA2_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA2S5_HAL);
}

void DMA2_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA2S6_HAL);
}

void DMA2_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(DMA2S7_HAL);
}
}
//##############################################################################

#endif

