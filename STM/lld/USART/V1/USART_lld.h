


#ifndef STM32F767_USART_H
#define STM32F767_USART_H

#include "board.h"
#include "BARE_DEFINES.h"
#include "RCC_lld.h"
#include "MUTEX.h"

struct USARTxo
{
	volatile USART_TypeDef * const USARTx;

	struct RCCxo RCCo;

	uint16_t unused3;

	struct MUTEX mutex;
};

extern struct USARTxo
	USART1o, 
	USART2o,
	USART3o,
	UART4o,
	UART5o,
	USART6o,
	UART7o,
	UART8o;


#define USART_CONFIG_ENABLED 1

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t USART_Config(struct USARTxo *USARTo, uint32_t CR1, uint32_t CR2, 
	uint32_t CR3, uint32_t BRR, uint32_t GTPR, uint32_t RTOR);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t USART_ResetConfig(struct USARTxo *USARTo);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void USART_Enable(struct USARTxo *USARTo);

#define USART_DISABLE_TRANSFER 1

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t USART_Disable(struct USARTxo *USARTo);


//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t USART_Get8(struct USARTxo *USARTo)
{
	uint32_t ret;
	ASM(" ldrb %0, [%1, #0x24]" :"=r" (ret) : "r" (USARTo->USARTx));
	return ret;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void USART_Put8(struct USARTxo *USARTo, uint32_t Data)
{
	ASM(" strb %1, [%0, #0x28]" ::"r" (USARTo->USARTx), "r" (Data));
}










void USART_D_GET(struct USARTxo *USARTo, uint8_t *data);
void USART_D_PUT(struct USARTxo *USARTo, uint8_t *data);

uint32_t USART_D_WRITE(struct USARTxo *USARTo, uint8_t *data, uint32_t num);
uint32_t USART_D_READ(struct USARTxo *USARTo, uint8_t *data, uint32_t num);














#endif
