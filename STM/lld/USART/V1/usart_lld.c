//
//
//
//
//





#include "usart_lld.h"

#define Usart4IrqHandler UART4_IRQHandler

#ifdef USART1
struct UsartObject	USART1_OBJECT = {{0x44,4,3}, 0, USART1};
#endif

#ifdef USART2
struct UsartObject	USART2_OBJECT = {{0x40,17,2}, 0, USART2};
#endif

#ifdef USART3
struct UsartObject	USART3_OBJECT = {{0x40,18,2}, 0, USART3};
#endif

#ifdef UART4
struct UsartObject	UART4_OBJECT = {{0x40,19,2}, 0, UART4};
#endif

#ifdef UART5
struct UsartObject	UART5_OBJECT = {{0x40,20,2}, 0, UART5};
#endif

#ifdef USART6
struct UsartObject	USART6_OBJECT = {{0x44,5,3}, 0, USART6};
#endif


//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t UsartConfig(
	const struct UsartObject * const usart_object,
	const struct UsartConfig * const usart_config)
{
	volatile USART_TypeDef * const usart = usart_object->usart;

	if((usart->CR1 & USART_CR1_UE) != 0) //if not zero. Usart is being used
	{
		return USARTCONFIG_ENABLED;
		//if usart is already enabled during config then it is already in use
	}

	uint32_t brr_mantissa = ClockGetPeripheralSpeed(&usart_object->rcc);
	//Get Peripheral Specific clock speed

	brr_mantissa = ((brr_mantissa >> 3) >> !usart_config->over8);
	brr_mantissa /= usart_config->clock_frequency;
	//solve for the brr mantissa.
	//equation is mantissa = clk / (8 * clock_frequency * (2 - over8))
	//first we take clk / 8 then we decide if we divide by 2 with the logical not
	//then we divide that number by the clock_frequency to get the mantissa

	usart->BRR = brr_mantissa << 4;
	usart->CR2 = usart_config->cr2;
	usart->CR3 = usart_config->cr3;
	usart->GTPR = usart_config->gtpr;
	usart->CR1 = usart_config->cr1 | USART_CR1_UE;
	//set Usart registers

	return 0;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t UsartResetConfig(
	const struct UsartObject * const usart_object)
{
	if((usart_object->usart->CR1 & USART_CR1_UE) == 0)
	{
		RccResetPeripheral(&usart_object->rcc);
		return 0;
	}
	return USARTCONFIG_ENABLED;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t UsartDisable(
	const struct UsartObject * const usart_object)
{	
	volatile USART_TypeDef * const usart = usart_object->usart;
	
	if((usart->SR & USART_SR_TC) != 0)
	{
		usart->CR1 &= ~USART_CR1_UE;
		
		return 0;
	}
	return USART_DISABLE_TRANSFER;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t UsartWrite8Buffer(
	const struct UsartObject * const usart_object,
	const struct CommunicationConfig * const communication_config)
{

	uint32_t num_data = communication_config->num_data;
	//get number of data

	uint8_t *data = communication_config->tx_data;


	Buffer8Write(&usart_object->tx_buffer,data,num_data);

	usart_object->usart->CR1 |= USART_CR1_TXEIE;
	//signal the peripheral by setting the interrupt enable bit
}

//******************************************************************************
//	
//										IRQ HANDLER FOR USARTS 
//	
//******************************************************************************
ALWAYS_INLINE void UsartInterruptHandler(struct UsartObject *usart_object)
{
	volatile USART_TypeDef *usart = usart_object->usart;
	//get usart structure

	volatile uint32_t flags = usart->SR;
	//get flags
	
	if((flags & USART_SR_TXE) != 0)
	//if TXE flag is set
	{
		uint8_t data = 0;

		if(Buffer8Get(&usart_object->tx_buffer, &usart->DR) == 0)
		//if no more data is left then turn off the interrupt
		{
			usart->CR1 &= ~USART_CR1_TXEIE;
			//turns off interrupt
		}
	}
	else if((flags & USART_SR_RXNE) != 0)
	//if RXNE is set
	{
		Buffer8Put(&usart_object->rx_buffer, &usart->DR);
		//read data from usart to buffer

		
		//generate event

	}
	else if( flags != 0)
	//rest of flags except zero
	{
		while(1)
			//usart error happened
			asm volatile("");
	}
}

#ifdef UART1
void Usart1IrqHandler(void)
{
	UsartInterruptHandler(&UART1_OBJECT);
}
#endif

#ifdef UART2
void Usart2IrqHandler(void)
{
	UsartInterruptHandler(&UART2_OBJECT);
}
#endif

#ifdef UART3
void Usart3IrqHandler(void)
{
	UsartInterruptHandler(&UART3_OBJECT);
}
#endif

#ifdef UART4
void Usart4IrqHandler(void)
{
	UsartInterruptHandler(&UART4_OBJECT);
}
#endif

#ifdef UART5
void Usart5IrqHandler(void)
{
	UsartInterruptHandler(&UART5_OBJECT);
}
#endif

#ifdef UART6
void Usart6IrqHandler(void)
{
	UsartInterruptHandler(&UART6_OBJECT);
}
#endif


