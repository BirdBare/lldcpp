//
//
//
//
//





#include "usart_lld.hpp"


#ifdef USART1
struct UsartHal	USART1_HAL = 
	{{&RCC->APB2ENR,
		RCC_APB2ENR_USART1EN,
		&RCC->APB2RSTR,
		RCC_PERIPHERAL_BUS_APB2},
		{1,
		 (IRQn_Type [1]){USART1_IRQn}},
		1,
		1,
		(uint8_t [1]){4},
		(uint8_t [1]){4},
		(DmaHal *[1]){&DMA2S7_HAL}, 
		(DmaHal *[1]){&DMA2S2_HAL},
		USART1};
#endif

/*
#ifdef USART2
struct UsartObject	USART2_OBJECT = 
	{{0x40,17,2}, 4,4, USART2, &DMA1S6_OBJECT, &DMA1S5_OBJECT};
#endif

#ifdef USART3
struct UsartObject	USART3_OBJECT = 
	{{0x40,18,2}, 4,4, USART3, &DMA1S3_OBJECT, &DMA1S1_OBJECT};
#endif

#ifdef UART4
struct UsartObject	UART4_OBJECT = 
	{{0x40,19,APB1}, 4,4, UART4, &DMA1S4_OBJECT, &DMA1S2_OBJECT};
#endif

#ifdef UART5
struct UsartObject	UART5_OBJECT = 
	{{0x40,20,2}, 4,4, UART5, &DMA1S7_OBJECT, &DMA1S0_OBJECT};
#endif

#ifdef USART6
struct UsartObject	USART6_OBJECT = 
	{{0x44,5,3}, 5,5, USART6, &DMA2S7_OBJECT, &DMA2S2_OBJECT};
#endif


//	
//										Usart Config 
//	
uint32_t UsartConfig(
	const struct UsartObject * const usart_object,
	const struct UsartConfig * const usart_config)
{
	volatile USART_TypeDef * const usart = usart_object->usart;

	if((usart->CR1 & USART_CR1_UE) != 0) //if not zero. Usart is being used
	{
		return 1;
		//if usart is already enabled during config then it is already in use
	}

	uint32_t brr_mantissa = LldClockGetPeripheralSpeed(&usart_object->rcc);
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
//##############################################################################

//	
//										 Usart Reset Config
//	
uint32_t UsartResetConfig(
	const struct UsartObject * const usart_object)
{
	if((usart_object->usart->CR1 & USART_CR1_UE) == 0)
	{
		RccResetPeripheral(&usart_object->rcc);
		return 0;
	}
	return 1;
}
//##############################################################################

//	
//										 Enable/Disable
//	
uint32_t UsartDisable(
	const struct UsartObject * const usart_object)
{	
	volatile USART_TypeDef * const usart = usart_object->usart;
	
	if((usart->SR & USART_SR_TC) != 0)
	{
		usart->CR1 &= ~USART_CR1_UE;
		
		return 0;
	}
	return 1;
}
//##############################################################################



//	
//										IRQ HANDLER FOR USARTS 
//	
ALWAYS_INLINE void UsartInterruptHandler(struct UsartObject *usart_object)
{
	volatile USART_TypeDef *usart = usart_object->usart;
	//get usart structure

	volatile uint32_t flags = usart->SR;
	//get flags

	if((flags & USART_SR_TXE) != 0)
	//if TXE flag is set
	{
		if(BufferGet8(&usart_object->tx_buffer, (uint8_t *)&usart->DR) == 0)
		//if no more data is left then turn off the interrupt
		{
			usart->CR1 &= ~USART_CR1_TXEIE;
			//turns off interrupt
		}
	}
	else if((flags & USART_SR_RXNE) != 0)
	//if RXNE is set
	{
		BufferPut8(&usart_object->rx_buffer, (uint8_t *)&usart->DR);
		//read data from usart to buffer
	}
	else if( flags != 0)
	//rest of flags except zero
	{
		while(1)
			//usart error happened
			asm volatile("");
	}
	
//	EventSignalFlags(&usart_object->event, flags);
	//generate event at the end
}

#ifdef USART1
void USART1_IRQHandler(void)
{
	UsartInterruptHandler(&USART1_OBJECT);
}
#endif

#ifdef USART2
void USART2_IRQHandler(void)
{
	UsartInterruptHandler(&USART2_OBJECT);
}
#endif

#ifdef USART3
void USART3_IRQHandler(void)
{
	UsartInterruptHandler(&USART3_OBJECT);
}
#endif

#ifdef UART4
void UART4_IRQHandler(void)
{
	UsartInterruptHandler(&UART4_OBJECT);
}
#endif

#ifdef UART5
void UART5_IRQHandler(void)
{
	UsartInterruptHandler(&UART5_OBJECT);
}
#endif

#ifdef USART6
void USART6_IRQHandler(void)
{
	UsartInterruptHandler(&USART6_OBJECT);
}
#endif
//##############################################################################
*/

