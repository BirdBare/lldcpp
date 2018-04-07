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


enum DMA_DATA_SIZE
{
	DMA_DATA_SIZE_8 = 0b00,
	DMA_DATA_SIZE_16 = 0b01,
	DMA_DATA_SIZE_32 = 0b10
};

enum DMA_HALF_TRANSFER_CALLBACK
{
	DMA_HALF_TRANSFER_CALLBACK_ENABLE = 1,
	DMA_HALF_TRANSFER_CALLBACK_DISABLE = 0
};

enum DMA_CIRCULAR
{
	DMA_CIRCULAR_ENABLE = 1,
	DMA_CIRCULAR_DISABLE = 0
};

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
class DmaObject;

struct DmaHal
{
	const struct RccHal rcc;
	const struct NvicHal nvic;

	const uint8_t flag_register_offset;
	const uint8_t flag_offset;

	volatile DMA_Stream_TypeDef * const dma;

	class DmaObject *owner = 0;

	uint32_t num_owners = 0;
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
	DMA2S6_HAL,
	DMA2S7_HAL;


//******************************************************************************
//
//
//
//******************************************************************************
struct DmaObjectSettings
{
	//LSB
		DMA_DATA_SIZE data_size:2; 
		//data size for the transfer

		DMA_HALF_TRANSFER_CALLBACK half_transfer_callback:1; 
		//call callback at half transfer instead of end of transfer

		uint32_t:5;

		DMA_CIRCULAR circular:1; //circular buffer mode
		
		uint32_t:7;
		
		DMA_PRIORITY priority:2;
		
		uint32_t:14;
		//MSB
};





//******************************************************************************
//
//
//
//******************************************************************************
static void LldDmaClearFlags(struct DmaHal *dma_object, uint32_t flags);



//******************************************************************************
//
//
//
//******************************************************************************
class DmaObject
{
	struct DmaHal *_hal;
	//hal dma object

	void (*_callback)(void *args) = 0;
	void *_args = 0;
	//callback settings

	struct DmaObjectSettings _settings = {
		DMA_DATA_SIZE_32,
		DMA_HALF_TRANSFER_CALLBACK_DISABLE,
		DMA_CIRCULAR_DISABLE,
		DMA_PRIORITY_VHIGH};

	uint32_t Config(void *par, void *m0ar, uint32_t num_data);

public:
	inline void (*GetCallback(void))(void *args) { return _callback; }
	inline void * GetCallbackArgs(void) { return _args; }
	inline class DmaObject * SetCallback(void (*callback)(void *args), void *args)
	{ _callback = callback; _args = args; return this; }
	inline class DmaObject * ResetCallback(void)
	{ _callback = 0; _args = 0; return this;}
	//get set and reset callback functions

	inline class DmaObject * SetSettings(uint32_t settings)
	{ *(uint32_t *)&_settings = settings; return this; }
	inline class DmaObject * SetSettings(struct DmaObjectSettings *settings)
	{ *(uint32_t *)&_settings = *(uint32_t *)settings; return this; }
	inline class DmaObject * ResetSettings()
	{ *(uint32_t *)&_settings = 0; return this; }
	//set and reset settings variable functions

	inline class DmaObject * SetDataSize(DMA_DATA_SIZE data_size)
	{ _settings.data_size = data_size; return this; }
	inline class DmaObject * EnableHalfTransferCallback(
		DMA_HALF_TRANSFER_CALLBACK half_transfer_callback)
	{ _settings.half_transfer_callback = half_transfer_callback; return this; }
	inline class DmaObject * EnableCircular(DMA_CIRCULAR circular)
	{ _settings.circular = circular; return this; }
	inline class DmaObject * SetPriority(DMA_PRIORITY priority)
	{ _settings.priority = priority; return this; }
	//set indiviual settings functions
	
	uint32_t Transfer(void *from, void *to, uint32_t length);
	uint32_t MemSet(void *address, uint32_t value, uint32_t length);
	uint32_t TransferP2M(void *from, void *to, uint32_t length);
	uint32_t TransferM2P(void *from, void *to, uint32_t length);
	//transfer mem2mem, periph2mem, and mem2periph

	DmaObject(DmaHal *hal)	
	{ 
		_hal = hal; 
		//set settings for dma object

		if(hal->num_owners++ == 0)
		{
			RccEnableClock(&_hal->rcc);
			NvicEnableHalInterrupt(&_hal->nvic);
		}
		//init the object
	}
		DmaObject(DmaHal *hal, uint32_t settings) 
	: DmaObject(hal)
	{
		*(uint32_t *)&_settings = settings;
	}
	DmaObject(DmaHal *hal, DmaObjectSettings settings) 
	: DmaObject(hal, *(uint32_t *)&settings)
	{}
	//dma object constructor

	~DmaObject()
	{
		if(--_hal->num_owners == 0)
		{
			NvicDisableHalInterrupt(&_hal->nvic);
		}
		//Deinit the object. clock isnt disabled because dma is always used
	}
	//dma object descructor
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

static uint32_t LldDmaGetFlags(struct DmaHal *dma_object)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_object->dma;
	//get dma stream

	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma


	uint32_t *flag_register = (uint32_t *)((uint32_t)&dma->LISR + 
		dma_object->flag_register_offset);


	return 0b111101 & (*flag_register >> dma_object->flag_offset);
	//return the flag register value
}

static void LldDmaClearFlags(struct DmaHal *dma_object, uint32_t flags)
{
	volatile DMA_Stream_TypeDef *dma_stream = dma_object->dma;
	//get dma stream

	volatile DMA_TypeDef *dma = (DMA_TypeDef *)((uint32_t)dma_stream & ~255);
	//get dma

	uint32_t *flag_register = (uint32_t *)((uint32_t)&dma->LIFCR + 
		dma_object->flag_register_offset);

		*flag_register = (0b111101 & flags) << dma_object->flag_offset;
	//return the flag register value
}
//##############################################################################








#ifdef __cplusplus
}
#endif







#endif
