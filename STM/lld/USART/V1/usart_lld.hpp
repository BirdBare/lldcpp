


#ifndef USART_HPP
#define USART_HPP

#include "board.h"
#include "rcc_lld.h"
#include "nvic_lld.h"
#include "dma_lld.hpp"



struct UsartSettings
{

	union
	{
		struct
		{
		};

		uint16_t cr1;
	};

union
	{
		struct
		{
		};

		uint16_t cr2;
	};

union
	{
		struct
		{
		};

		uint16_t cr3;
	};



};



//******************************************************************************
//	
//									Usart Definitions	 
//	
//******************************************************************************
class UsartObject;

struct UsartHal
{
	struct RccHal rcc;
	struct NvicHal nvic;

	uint8_t num_tx_dma;
	uint8_t num_rx_dma;

	uint8_t *tx_dma_channel;
	uint8_t *rx_dma_channel;

	struct DmaHal **tx_dma; //transmission dma
	struct DmaHal **rx_dma; //reception dma

	volatile USART_TypeDef * const usart;	

	class UsartObject *owner = 0;

	uint32_t num_owners = 0;
}; 

extern struct UsartHal
	USART1_HAL, 
	USART2_HAL,
	USART3_HAL,
	UART4_HAL,
	UART5_HAL,
	USART6_HAL,
	UART7_HAL,
	UART8_HAL;



//******************************************************************************
//
//
//
//******************************************************************************
class UsartObject
{
protected:
	struct UsartHal &_hal;

	struct UsartSettings _settings;

	uint16_t _brr;
	uint16_t _gtpr;

public:
	inline UsartHal& Hal(void) { return _hal; };

	inline UsartSettings& Settings(void) {return _settings; }

	UsartObject(UsartHal &hal, const UsartSettings &settings)
	: _hal(hal), _settings(settings)
	{
			
		if(_hal.num_owners++ == 0)
		{
			NvicEnableHalInterrupt(&hal.nvic);
			RccEnableClock(&hal.rcc);
		}
	}

	~UsartObject()
	{
		if(--_hal.num_owners == 0)
		{
			RccResetPeripheral(&_hal.rcc);
			NvicDisableHalInterrupt(&_hal.nvic);
			RccDisableClock(&_hal.rcc);
		}
	}
};




/*
struct UsartConfig
{
	uint32_t clock_frequency; //desired clock freq in bits per second (bps)
	
	union
	{
		uint16_t cr1; //control register 1
		
		struct
		{
//LSB
			uint16_t sbk:1; //send break character enable
			uint16_t rwu:1; //Receiver wakeup. Mute mode
#define RWU_ACTIVE 0
#define RWU_MUTE 1

			uint16_t:2; //Receiver enable
			uint16_t idleie:1; //Idle interrupt enable
			uint16_t:4; //Receiver enable
			uint16_t ps:1; //Parity selection bit
#define PS_EVEN 0
#define PS_ODD 1

			uint16_t pce:1; //Parity Control enable
			uint16_t wake:1; //Wakeup Method
#define WAKE_IDLELINE 0
#define WAKE_ADDRESS 1
			
			uint16_t m:1; //Word length
#define M_8BIT 0
#define M_9BIT 1

			uint16_t :1; //Usart Enable. Always set on Configuration. No need for bit
			uint16_t :1;
			uint16_t over8:1; //Oversampling Mode
#define OVER8_16 0
#define OVER8_8 1
			//MSB
		};
	};

	union
	{
		uint16_t cr2; //control register 2

//LSB
	struct
	{
			uint16_t add:4; //Address of Usart node
			uint16_t :1;
			uint16_t lbdl:1; //LIN break detection length
#define LBDL_10_BIT 0
#define LBDL_11_BIT 1

			uint16_t lbdie:1; //LIN break detection interrupt enable
			uint16_t :1;
			uint16_t lbcl:1; //Last Bit clock output pulse enable
			uint16_t cpha:1; //Clock output Phase
#define CPHA_FIRST 0
#define CPHA_SECOND 1

			uint16_t cpol:1; //Clock output Idle Polarity
#define CPOL_LOW 0
#define CPOL_HIGH 1

			uint16_t clken:1; //Clock output enable
			uint16_t stop:2; //Number of stop bits
#define STOP_1_BIT 0
#define STOP_0POINT5_BIT 1
#define STOP_2_BIT 2
#define STOP_1POINT5_BIT 3

			uint16_t linen:1; //LIN mode enable
			uint16_t :1;
			//MSB
		};
	};

	union
	{
			uint16_t cr3; //control register 3

			struct 
			{

			
			//LSB
			uint16_t eie:1; //Error interrupt enable
			uint16_t iren:1; //IrDA mode enable
			uint16_t irlp:1; //IrDA power mode select
#define IRLP_NORMAL_POWER 0
#define IRLP_LOW_POWER 1

			uint16_t hdsel:1; //Half duplex mode enable
			uint16_t nack:1; //Smartcard NACK enable when parity error happens
			uint16_t scen:1; //Smartcard mode enable
			uint16_t:2; //DMA enable for receiver
			uint16_t rtse:1; //rts enable. Only get data when receive buffer empty
			uint16_t ctse:1; //cts input enable. Only transmit when CTS is low (0)
			uint16_t ctsie:1; //cts interrupt enable
			uint16_t onebit:1; //Sample method for data
			uint16_t :4;
#define ONEBIT_3_BIT 0
#define ONEBIT_1_BIT 1
			//MSB
		};



	};

	union
	{
			uint16_t gtpr; //Guard time and prescaler register
		struct
		{
						//LSB
			uint16_t psc:8; //IrDA low power clock div. OR Smartcard clock div * 2
											//In normal IrDA mode must be set to 1
			uint16_t gt:8; //SmartCard Gaurd Time in number of baud clock ticks
			//MSB
		};
	};
};

//##############################################################################



//******************************************************************************
//	
//									Usart Configure functions	 
//	
//******************************************************************************

uint32_t UsartConfig(
	const struct UsartObject * const usart_object,
	const struct UsartConfig * const usart_config);

uint32_t UsartResetConfig(
	const struct UsartObject * const usart_object);


static void UsartSendWord(struct UsartObject *usart_object,char *word)
{
	uint32_t counter = 0;

	do
	{
		do
		{
			asm volatile("");
		}while((usart_object->usart->SR & USART_SR_TXE) == 0);

		usart_object->usart->DR = word[counter];

	}while(word[++counter] != '\0');
}

static void UsartSendNumber(struct UsartObject *usart_object,uint32_t number)
{
	uint32_t counter = 0;
	uint8_t numbers[12];

	do
	{
		numbers[counter++] = '0' + (number % 10);
		number /= 10;
	
	} while(number != 0 && counter < 12);

	do
	{
		do
		{
			asm volatile("");
		}while((usart_object->usart->SR & USART_SR_TXE) == 0);

		usart_object->usart->DR = numbers[--counter];

	}while(counter != 0);
}

static uint32_t GetBitNumber(uint32_t bit)
{
	uint32_t count = 0;
	
	while((bit & (0b1 << count)) == 0)
	{
		count++;
	}

	return count;
}

static void UsartSendNumberBinary(struct UsartObject *usart_object,uint32_t number)
{
	uint32_t counter = 31;
	
	UsartSendWord(usart_object,"0b");
	
	do
	{
		do
		{
			asm volatile("");
		}while((usart_object->usart->SR & USART_SR_TXE) == 0);

		usart_object->usart->DR = '0' + !!(number & (0b1 << counter));

	}while(counter-- != 0);
}

//##############################################################################

//******************************************************************************
//	
//									Usart Enable/Disable Functions	 
//	
//******************************************************************************
uint32_t UsartDisable(
	const struct UsartObject * const usart_object);

//##############################################################################

******************************************************************************
//	
//										 Initialize any USART with its object
//	
******************************************************************************
ALWAYS_INLINE void UsartInitInterrupt(struct UsartObject *usart_object,
	void *tx_buffer_mem, uint32_t tx_buffer_size,
	void *rx_buffer_mem, uint32_t rx_buffer_size)
{
	RccEnableClock(&usart_object->rcc);

	usart_object->tx_buffer.buffer = tx_buffer_mem;
	usart_object->tx_buffer.buffer_size = tx_buffer_size;

	usart_object->rx_buffer.buffer = rx_buffer_mem;
	usart_object->rx_buffer.buffer_size = rx_buffer_size;

	struct UsartConfig usart_config = {0};
	usart_config.clock_frequency = 9600;
	usart_config.te = 1;
	usart_config.re = 1;

	UsartConfig(usart_object, &usart_config);
}

//##############################################################################

******************************************************************************
//	
//									Usart Read/Write functions buffer interrupt driven	 
//	
******************************************************************************
ALWAYS_INLINE void UsartPutBuffer(
	const struct UsartObject * const usart_object,
	char *character)
{
	while(BufferPut8((struct Buffer *)&usart_object->tx_buffer,
		(uint8_t *)character) == 0)
	{
		;
	}

	usart_object->usart->CR1 |= USART_CR1_TXEIE;
}

ALWAYS_INLINE void UsartGetBuffer(
	const struct UsartObject * const usart_object,
	char *character)
{
	BufferGet8((struct Buffer *)&usart_object->tx_buffer, (uint8_t *)character);
}

ALWAYS_INLINE void UsartWriteBuffer(
	const struct UsartObject * const usart_object,
	uint8_t *string, uint32_t size)
{
	uint32_t counter = 0;

	do
	{
		UsartPutBuffer(usart_object,&string[counter]);
		counter++;
	} while(counter < size);
}

ALWAYS_INLINE void UsartWriteBufferString(
	const struct UsartObject * const usart_object,
	char *string)
{
	uint32_t counter = 0;

	do
	{
		UsartPutBuffer(usart_object,&string[counter]);
		counter++;
	} while(string[counter] != '\0');
}

ALWAYS_INLINE void UsartReadBuffer(
	const struct UsartObject * const usart_object,
	char *string)
{


}
//##############################################################################


*/

#endif
