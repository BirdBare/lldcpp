//
//
//
//
//



#include "dma_lld.hpp"


struct DmaHal
	DMA1S0_HAL = {
		{0x30,
		21,
		AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream0_IRQn}},
		0,
		0,
		DMA1_Stream0};

struct DmaHal
DMA1S1_HAL = {
		{0x30,
		21,
		AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream1_IRQn}},
		0,
		6,
		DMA1_Stream1};

struct DmaHal
DMA1S2_HAL = {
		{0x30,
		21,
		AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream2_IRQn}},
		0,
		16,
		DMA1_Stream2};

struct DmaHal
DMA1S3_HAL = {
		{0x30,
		21,
		AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream3_IRQn}},
		0,
		22,
		DMA1_Stream3};

struct DmaHal
DMA1S4_HAL = {
		{0x30,
		21,
		AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream4_IRQn}},
		4,
		0,
		DMA1_Stream4};

struct DmaHal
DMA1S5_HAL = {
		{0x30,
		21,
		AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream5_IRQn}},
		4,
		6,
		DMA1_Stream5};

struct DmaHal
DMA1S6_HAL = {
		{0x30,
		21,
		AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream6_IRQn}},
		4,
		16,
		DMA1_Stream6};

struct DmaHal
DMA1S7_HAL = {
		{0x30,
		21,
		AHB},
		{1,
		(IRQn_Type[1]){DMA1_Stream7_IRQn}},
		4,
		22,
		DMA1_Stream7};

struct DmaHal
DMA2S0_HAL = {
		{0x30,
		22,
		AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream0_IRQn}},
		0,
		0,
		DMA2_Stream0};

struct DmaHal
DMA2S1_HAL = {
		{0x30,
		22,
		AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream1_IRQn}},
		0,
		6,
		DMA2_Stream1};

struct DmaHal
DMA2S2_HAL = {
		{0x30,
		22,
		AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream2_IRQn}},
		0,
		16,
		DMA2_Stream2};

struct DmaHal
DMA2S3_HAL = {
		{0x30,
		22,
		AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream3_IRQn}},
		0,
		22,
		DMA2_Stream3};

struct DmaHal
DMA2S4_HAL = {
		{0x30,
		22,
		AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream4_IRQn}},
		4,
		0,
		DMA2_Stream4};

struct DmaHal
DMA2S5_HAL = {
		{0x30,
		22,
		AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream5_IRQn}},
		4,
		6,
		DMA2_Stream5};

struct DmaHal
DMA2S6_HAL = {
		{0x30,
		22,
		AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream6_IRQn}},
		4,
		16,
		DMA2_Stream6};

struct DmaHal
DMA2S7_HAL = {
		{0x30,
		22,
		AHB},
		{1,
		(IRQn_Type[1]){DMA2_Stream7_IRQn}},
		4,
		22,
		DMA2_Stream7};







//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaObject::Config(void *par, void *m0ar, uint32_t length)
{
	LldDmaClearFlags(_hal, 0b111101);
	//clear flags first

	_hal->dma->NDTR = length;
	//set num data to send

	_hal->dma->PAR = (uint32_t)par;
	//peripher address or memory area to send(tx memory)

	_hal->dma->M0AR = (uint32_t)m0ar;
	//should be memory area to receive or send the data

	if(_callback != 0)
	{
		_hal->owner = this;
		//set owner because we will call callback at transfer complete

		if(_settings.half_transfer_callback == DMA_HALF_TRANSFER_CALLBACK_ENABLE)
		{
			return DMA_SxCR_HTIE;
		}

		return DMA_SxCR_TCIE;
	}

	return 0;
}


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaObject::Transfer(void *from, void *to, uint32_t length)
{
	_hal->dma->FCR = DMA_SxFCR_FEIE | DMA_SxFCR_DMDIS |
		DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1;
	//set error interrupt

	_hal->dma->CR = *(uint32_t *)&_settings | Config(from,to,length) |
		DMA_SxCR_TEIE | DMA_SxCR_DMEIE | _settings.data_size << 11 |
		_settings.data_size << 13 | DMA_SxCR_MINC | DMA_SxCR_PINC |
		DMA_SxCR_DIR_1 | DMA_SxCR_MBURST_0 | DMA_SxCR_PBURST_0 | DMA_SxCR_EN;
	//config the addresses first return value is used in cr register
	//set error interrupts and other needed stuff

	return 0;
}



//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaObject::MemSet(void *address, uint32_t value = 0, uint32_t length = 0)
{
	_hal->dma->FCR = DMA_SxFCR_FEIE | DMA_SxFCR_DMDIS |
		DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1;
	//set error interrupt

	_hal->dma->CR = *(uint32_t *)&_settings | Config(&value,address,length) |
		DMA_SxCR_TEIE | DMA_SxCR_DMEIE | _settings.data_size << 11 |
		_settings.data_size << 13 | DMA_SxCR_MINC | DMA_SxCR_DIR_1 | 
		DMA_SxCR_MBURST_0 | DMA_SxCR_PBURST_0 | DMA_SxCR_EN;
	//config the addresses first return value is used in cr register
	//set error interrupts and other needed stuff

	return 0;
}



//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaObject::TransferP2M(void *from, void *to, uint32_t length)
{
	_hal->dma->FCR = DMA_SxFCR_FEIE;
	//set error interrupt

	_hal->dma->CR = *(uint32_t *)&_settings | Config(from,to,length) |
		DMA_SxCR_TEIE | DMA_SxCR_DMEIE | _settings.data_size << 11 | 
		_settings.data_size << 13 | DMA_SxCR_MINC | DMA_SxCR_EN;
	//set error interrupts and other needed stuff 

	return 0;
} 


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t DmaObject::TransferM2P(void *from, void *to, uint32_t length)
{
	_hal->dma->FCR = DMA_SxFCR_FEIE;
	//set error interrupt

	_hal->dma->CR = *(uint32_t *)&_settings | Config(to,from,length) |
		DMA_SxCR_TEIE | DMA_SxCR_DMEIE | _settings.data_size << 11 | 
		_settings.data_size << 13 | DMA_SxCR_MINC | DMA_SxCR_EN;
	//set error interrupts and other needed stuff 

	return 0;
} 
//transfer mem2mem, periph2mem, and mem2periph













//******************************************************************************
//	
//									DMA IRQ HANDLERS	 
//	
//******************************************************************************

static inline void DMA_STREAM_HANDLER(struct DmaHal *dma_object)
{
	uint32_t flags = LldDmaGetFlags(dma_object);

	if((flags & DMA_ISR_FEIF) != 0)
		BREAK(1);
	if((flags & DMA_ISR_DMEIF) != 0)
		BREAK(2);
	if((flags & DMA_ISR_TEIF) != 0)
		BREAK(3);

		if((((flags & DMA_ISR_TCIF) != 0 && (dma_object->dma->CR & DMA_SxCR_TCIE) != 
		0) || ((flags & DMA_ISR_HTIF) != 0 && (dma_object->dma->CR & DMA_SxCR_HTIE) !=
		0)))
	//if transfer complete flag and interrupt enable bit is set or
	//if half transfer complete flag and interrupt enable bit is set and
	//if callback is set. then we call the callback that was set.
	{
		dma_object->owner->GetCallback()(dma_object->owner->GetCallbackArgs());
		dma_object->owner = 0;
		//call callback
	}

	LldDmaClearFlags(dma_object, flags);

	return;
}

#ifdef DMA1
void DMA1_Stream0_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S0_HAL);
}
void DMA1_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S1_HAL);
}

void DMA1_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S2_HAL);
}

void DMA1_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S3_HAL);
}

void DMA1_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S4_HAL);
}

void DMA1_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S5_HAL);
}

void DMA1_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S6_HAL);
}

void DMA1_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA1S7_HAL);
}

#endif

#ifdef DMA2
void DMA2_Stream0_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S0_HAL);
}

void DMA2_Stream1_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S1_HAL);
}

void DMA2_Stream2_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S2_HAL);
}

void DMA2_Stream3_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S3_HAL);
}

void DMA2_Stream4_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S4_HAL);
}

void DMA2_Stream5_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S5_HAL);
}

void DMA2_Stream6_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S6_HAL);
}

void DMA2_Stream7_IRQHandler(void) 
{
	DMA_STREAM_HANDLER(&DMA2S7_HAL);
}
//##############################################################################

#endif

