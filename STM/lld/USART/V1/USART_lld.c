//
//
//
//
//





#include "USART_lld.h"
#include "RCC_lld.h"
#include "PERIPH.h"

#define USART_DIRECT_FUNCTIONS \
	(void *)USART_D_PUT, \
	(void *)USART_D_GET, \
	(void *)USART_D_WRITE, \
	(void *)USART_D_READ


struct USARTxo
	USART1o = {USART1, {0x44,4}},
	USART2o = {USART2, {0x40,17}},
	USART3o = {USART3, {0x40,18}},
	UART4o = {UART4, {0x40,19}},
	UART5o = {UART5, {0x40,20}},
	USART6o = {USART6, {0x44,5}},
	UART7o = {UART7, {0x40,30}},
	UART8o = {UART8, {0x40,31}};



//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t USART_Config(struct USARTxo *USARTo, uint32_t CR1, uint32_t CR2, 
	uint32_t CR3, uint32_t BRR, uint32_t GTPR, uint32_t RTOR)
{
	volatile USART_TypeDef * const USARTx = USARTo->USARTx;

	if((USARTx->CR1 & USART_CR1_UE) == 0)
	{
		USARTx->CR2 = CR2;
		USARTx->CR3 = CR3;
		USARTx->BRR = BRR;
		USARTx->GTPR = GTPR;
		USARTx->RTOR = RTOR;
		USARTx->CR1 = CR1;
		return 0;
	}
	return USART_CONFIG_ENABLED;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t USART_ResetConfig(struct USARTxo *USARTo)
{
	if((USARTo->USARTx->CR1 & USART_CR1_UE) == 0)
	{
		RCC_Reset((struct RCCxo *)USARTo);
		return 0;
	}
	return USART_CONFIG_ENABLED;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void USART_Enable(struct USARTxo *USARTo)
{
	USARTo->USARTx->CR1 |= USART_CR1_UE;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t USART_Disable(struct USARTxo *USARTo)
{	
	volatile USART_TypeDef * const USARTx = USARTo->USARTx;
	
	if((USARTx->ISR & USART_ISR_TC) != 0)
	{
		USARTo->USARTx->CR1 &= ~USART_CR1_UE;
		
		return 0;
	}
	return USART_DISABLE_TRANSFER;
}


//##############################################################################

void USART_D_PUT(struct USARTxo *USARTo, uint8_t *data)
{
	PERIPH_WaitTillSet(&USARTo->USARTx->ISR, USART_ISR_TXE);
	USART_Put8(USARTo, *data);
}

void USART_D_GET(struct USARTxo *USARTo, uint8_t *data)
{
	PERIPH_WaitTillSet(&USARTo->USARTx->ISR, USART_ISR_RXNE);
	*data = USART_Get8(USARTo);
}

uint32_t USART_D_WRITE(struct USARTxo *USARTo, uint8_t *data, uint32_t num)
{
	uint32_t count = 0;

	volatile uint32_t *ISR = &USARTo->USARTx->ISR;
	
	do
	{
		PERIPH_WaitTillSet(ISR, USART_ISR_TXE);
		USART_Put8(USARTo, data[count++]);
	} while(count < num);
	
	return count;
}

uint32_t USART_D_READ(struct USARTxo *USARTo, uint8_t *data, uint32_t num)
{
	uint32_t count = 0;

	volatile uint32_t *ISR = &USARTo->USARTx->ISR;
	
	do
	{
		PERIPH_WaitTillSet(ISR, USART_ISR_RXNE);
		data[count++] = USART_Get8(USARTo);
	} while(count < num);
	
	return count;
}













