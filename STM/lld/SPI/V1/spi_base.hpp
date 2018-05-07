//
//
//
//
//



#ifndef SPI_BASE_HPP
#define SPI_BASE_HPP

#include "board.h"
#include "rcc_lld.h"
#include "nvic_lld.h"
#include "dma_lld.hpp"




//******************************************************************************
//	
//		Spi Hal struct
//	-----------------
//	Description: struct that hold all the information needed to run this 
//		peripheral using this driver. Everything from clock information to dma
//		information is included in this struct to abstract away from the user.
//
//******************************************************************************
struct SpiHal
{
	struct RccHal rcc; //clock control 
	struct NvicHal nvic; //interrupt control 

	// --- DMA CONTROL ---
	uint8_t num_tx_dma; 
	uint8_t num_rx_dma;

	uint8_t *tx_dma_channel; 
	uint8_t *rx_dma_channel;

	struct DmaHal **tx_dma;
	struct DmaHal **rx_dma;
	// --- END DMA CONTROL ---

	volatile SPI_TypeDef * const spi; //physical address of peripheral registers

	class SpiBase *owner = 0; //driver that is currently using the peripheral

	uint32_t num_owners = 0; //number of drivers currently connected to peripheral
};

//Definition of Spi Hal infos
extern struct SpiHal
	SPI1_HAL,
	SPI2_HAL,
	SPI3_HAL,
	SPI4_HAL,
	SPI5_HAL,
	SPI6_HAL;



//******************************************************************************
//	
//
//
//******************************************************************************
class SpiBase
{
protected:
	SpiHal &_hal; //info object for driver

	void (*_interrupt)(void *interrupt_args); //interrupt for drivers
	void *_interrupt_args; //args for interrupt

	void (*_callback)(void *callback_args); //callback for drivers
	void *_callback_args; //callback args

	SpiBase(SpiHal &hal);
	
	~SpiBase();

public:
	
	//Get Hal info struct
	SpiHal& Hal(void);

	void (*Callback(void))(void *args) ;
	void Callback(void (*callback)(void *args));
	void* CallbackArgs(void) ;
	void CallbackArgs(void *args) ;
	void ResetCallback(void) ;

	void (*Interrupt(void))(void *args) ;
	void* InterruptArgs(void) ;
};



//******************************************************************************
//
//
//
//******************************************************************************
	inline SpiBase::SpiBase(SpiHal &hal)
	: _hal(hal), _interrupt(0), _interrupt_args(this), 
		_callback(0), _callback_args(0)
	{
		if(hal.num_owners++ == 0)
		{
			NvicEnableHalInterrupt(&hal.nvic);
			RccEnableClock(&hal.rcc);
		}
		//init the object if it has never been connected
	}
	

//******************************************************************************
//
//
//
//******************************************************************************
	inline SpiBase::~SpiBase()
	{
		if(--_hal.num_owners == 0)
		{
			RccResetPeripheral(&_hal.rcc);
			NvicDisableHalInterrupt(&_hal.nvic);
			RccDisableClock(&_hal.rcc);
		}
		//simple deinit object
	}


//******************************************************************************
//
//
//
//******************************************************************************
	inline SpiHal& SpiBase::Hal(void) 
	{
		return _hal;
	}



//******************************************************************************
//
//
//
//******************************************************************************
	inline void (*SpiBase::Callback(void))(void *args) 
	{ 
		return _callback;
	}
	inline void SpiBase::Callback(void (*callback)(void *args))
	{
		_callback = callback;
	}


//******************************************************************************
//
//
//
//******************************************************************************
	inline void * SpiBase::CallbackArgs(void) 
	{ 
		return _callback_args; 
	}
	inline void SpiBase::CallbackArgs(void *args)
	{
		_callback_args = args;
	}


//******************************************************************************
//
//
//
//******************************************************************************
	inline void SpiBase::ResetCallback(void) 
	{
		Callback(0);
		CallbackArgs(0);
	}
	//call get and set Callback



//******************************************************************************
//
//
//
//******************************************************************************
	inline void (*SpiBase::Interrupt(void))(void *args) 
	{ 
		return _interrupt; 
	}


//******************************************************************************
//
//
//
//******************************************************************************
	inline void * SpiBase::InterruptArgs(void) 
	{ 
		return _interrupt_args; 
	}




#endif








