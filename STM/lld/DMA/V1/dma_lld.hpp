//
//
//
//
//



#ifndef STM32F767_DMA_H
#define STM32F767_DMA_H


#include "board.h"
#include "rcc_lld.h"
#include "nvic_lld.h"

#ifdef __cplusplus
extern "C" {
#endif

//to get DMA Base address. Take DMA_Stream & ~255.


enum DMA_PRIORITY
{
	DMA_PRIORITY_LOW = 0b00,
	DMA_PRIORITY_MED = 0b01,
	DMA_PRIORITY_HIGH = 0b10,
	DMA_PRIORITY_VHIGH = 0b11
};

enum DMA_CHANNEL
{
	DMA_CHANNEL_0 = 0b000,
	DMA_CHANNEL_1 = 0b001,
	DMA_CHANNEL_2 = 0b010,
	DMA_CHANNEL_3 = 0b011,
	DMA_CHANNEL_4 = 0b100,
	DMA_CHANNEL_5 = 0b101,
	DMA_CHANNEL_6 = 0b110,
	DMA_CHANNEL_7 = 0b111
};



//******************************************************************************
//
//
//
//******************************************************************************

struct DmaHal
{
	//information structure
	const struct RccHal rcc;
	const struct NvicHal nvic;

	const uint8_t flag_register_offset;
	const uint8_t flag_offset;

	volatile DMA_Stream_TypeDef * const dma;
	//end information

	volatile class DmaObject *owner = 0;

	volatile uint32_t num_connected = 0;
}; 

extern struct DmaHal 
	DMA1S0_HAL,
	DMA1S1_HAL,
	DMA1S2_HAL,
	DMA1S3_HAL,
	DMA1S4_HAL,
	DMA1S5_HAL,
	DMA1S6_HAL,
	DMA1S7_HAL,

	DMA2S0_HAL,
	DMA2S1_HAL,
	DMA2S2_HAL,
	DMA2S3_HAL,
	DMA2S4_HAL,
	DMA2S5_HAL,
	DMA2S6_HAL,
	DMA2S7_HAL;


//******************************************************************************
//
//
//
//******************************************************************************
struct DmaSettings
{
	//Operator =
	DmaSettings& operator=(const DmaSettings &copy)
	{	
		*(uint32_t *)this = *(uint32_t *)&copy; 
		return *this; 
	}

	//Set and Get Datasize
	uint32_t DataSize(void) 
	{
		return 8 << data_size; 
	}
	DmaSettings& DataSize(uint32_t size) 
	{
		data_size = (size + 7) >> 4; 
		return *this;
	}
	
	//Set and Get HTC
	bool HalfTransferCallback(void) 
	{
		return half_transfer_callback; 
	}
	DmaSettings& HalfTransferCallback(bool state) 
	{
		half_transfer_callback = state; 
		return *this;
	}
	
	//Set and Get Ciruclar
	bool Circular(void) 
	{
		return circular; 
	}
	DmaSettings& Circular(bool state) 
	{
		circular = state; 
		return *this;
	}

	//SEt and Get Priority
	DMA_PRIORITY Priority(void) 
	{	
		return priority;
	}
	DmaSettings& Priority(DMA_PRIORITY prio) 
	{
		priority = prio; 
		return *this;
	}

	//Set and Get Channel
	uint32_t Channel(void) 
	{
		return channel;
	}
	DmaSettings& Channel(uint32_t chan) 
	{
		channel = chan; 
		return *this;
	}

	union
	{
		struct 
		{
		//LSB
		uint32_t data_size:2; 
		//data size for the transfer

		bool half_transfer_callback:1; 
		//call callback at half transfer instead of end of transfer

		uint32_t:5;

		bool circular:1; //circular buffer mode
		
		uint32_t:7;
		
		DMA_PRIORITY priority:2;
		
		uint32_t:7;

		uint32_t channel:3;

		uint32_t:4;
		//MSB
		};
		
		uint32_t cr = 0b10;
	};
};


//******************************************************************************
//
//
//
//******************************************************************************
class DmaObject
{
protected:
	struct DmaHal &_hal;
	//hal dma object

	struct DmaSettings _settings; 
	//settings for the dma

	void PreTransmission(void *par, void *m0ar, uint32_t num_data);
	//a pre transmission phase for all transfer types

public:
	//Get Hal info structure
	DmaHal& Hal(void) 
	{ 
		return _hal;
	}

	//Lock and Unlock Hal info
	void Lock(void) 
	{ 
		if(_hal.owner == 0) 
			_hal.owner = this; 
		else 
			BREAK(0); 	
	}
	void Unlock(void) 
	{ 
		if(_hal.owner == this) 
			_hal.owner = 0; 
		else 
			BREAK(0); 
	}

	//Get Object Settings
	DmaSettings& Settings(void) 
	{	
		return _settings; 
	}

	//Get Status of Operation
	uint32_t Status(void) 
	{ 
		return _hal.dma->CR & DMA_SxCR_EN; 
	}

	//Stop Current Operation immediately
	void Stop(void) 
	{ 
		_hal.dma->CR &= ~DMA_SxCR_EN; 
	}

	uint32_t Transfer(void *from, void *to, uint32_t length);
	uint32_t MemSet(void *address, uint32_t value, uint32_t length);
	//transfer mem2mem, periph2mem, and mem2periph

	DmaObject(DmaHal &hal, const DmaSettings &settings)	
	: _hal(hal), _settings(settings)
	{ 
		if(_hal.num_connected++ == 0)
		{
			RccEnableClock(&_hal.rcc);
			NvicEnableHalInterrupt(&_hal.nvic);
		}
		//init the object
	}
	//dma object constructor

	~DmaObject()
	{
		if(--_hal.num_connected == 0)
		{
			NvicDisableHalInterrupt(&_hal.nvic);
		}
		//Deinit the object. clock isnt disabled because dma is always used
	}
	//dma object descructor
};








class DmaPolled : public DmaObject
{
	void WaitTransfer(void) 
	{ 
		while(Status() != 0) { NOP; } 
	}

public:
	uint32_t MemSet(void *address, uint32_t value = 0, uint32_t length = 1)
	{
		DmaObject::MemSet(address, value,length);

		_hal.dma->CR |= DMA_SxCR_EN;

		WaitTransfer();

		return 0;
	}
	uint32_t Transfer(void *from, void *to, uint32_t length = 1)
	{
		DmaObject::Transfer(from, to,length);

		_hal.dma->CR |= DMA_SxCR_EN;

		WaitTransfer();

		return 0;
	}

	DmaPolled(DmaHal &hal, const DmaSettings &settings = {})
	: DmaObject(hal, settings)
	{}
};


class DmaInterrupt : public DmaObject
{
	void (*_callback)(void *args) = 0;
	void *_args = 0;

	uint32_t CheckInterrupt(void) 
	{
		if(_callback != 0)
		{
			if(_settings.half_transfer_callback == true)
			{
			return DMA_SxCR_HTIE;
			}

			return DMA_SxCR_TCIE;
		}
		return 0;
	}

public:
	void (*GetCallback(void))(void *args) 
	{ 
		return _callback; 
	}
	void * GetCallbackArgs(void) 
	{ 
		return _args; 
	}
	void SetCallback(void (*callback)(void *args), void *args) 
	{	
		_callback = callback; 
		_args = args; 
	}
	void ResetCallback(void) 
	{ 
		_callback = 0; 
		_args = 0; 
	}
	//get set and reset callback functions for objects that implement callback


	uint32_t MemSet(void *address, uint32_t value = 0, uint32_t length = 1)
	{
		DmaObject::MemSet(address, value,length);

		_hal.dma->CR |= DMA_SxCR_EN | CheckInterrupt();

		return 0;
	}
	uint32_t Transfer(void *from, void *to, uint32_t length = 1)
	{
		DmaObject::Transfer(from, to,length);

		_hal.dma->CR |= DMA_SxCR_EN | CheckInterrupt();

		return 0;
	}

	uint32_t TransferP2M(void *from, void *to, uint32_t length);
	uint32_t TransferM2P(void *from, void *to, uint32_t length);

	DmaInterrupt(DmaHal &hal, const DmaSettings &settings = {})
	: DmaObject(hal, settings)
	{}

};



//******************************************************************************
//
//
//
//******************************************************************************


#define DMA_ISR_FEIF 0b1
#define DMA_ISR_DMEIF 0b100
#define DMA_ISR_TEIF 0b1000
#define DMA_ISR_HTIF 0b10000
#define DMA_ISR_TCIF 0b100000

//******************************************************************************
//	
//										Dma Get/Clear flags 
//	
//******************************************************************************

static uint32_t LldDmaGetFlags(struct DmaHal &dma_object)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_object.dma;
	//get dma stream

	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma


	uint32_t *flag_register = (uint32_t *)((uint32_t)&dma->LISR + 
		dma_object.flag_register_offset);


	return 0b111101 & (*flag_register >> dma_object.flag_offset);
	//return the flag register value
}

static void LldDmaClearFlags(struct DmaHal &dma_object, uint32_t flags)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_object.dma;
	//get dma stream

	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma

	uint32_t *flag_register = (uint32_t *)((uint32_t)&dma->LIFCR + 
		dma_object.flag_register_offset);

		*flag_register = (0b111101 & flags) << dma_object.flag_offset;
	//return the flag register value
}
//##############################################################################








#ifdef __cplusplus
}
#endif







#endif
