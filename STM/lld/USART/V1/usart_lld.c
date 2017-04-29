//
//
//
//
//





#include "usart_lld.h"

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
	brr_mantissa /= usart_config->baud_rate;
	//solve for the brr mantissa.
	//equation is mantissa = clk / (8 * baud * (2 - over8))
	//first we take clk / 8 then we decide if we divide by 2 with the logical not
	//then we divide that number by the baud rate to get the mantissa

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
uint32_t UsartPut8Timeout(
	const struct UsartObject * const usart_object,
	const uint8_t * data,
	uint32_t num_data,
	const uint32_t timeout_milli)
{
	const volatile USART_TypeDef * usart = usart_object->usart;

	const uint32_t milli_ref = SysTickGetMilli();

	do
	{
		while((usart->SR & USART_SR_TXE) == 0)
		{
			if((SysTickGetMilli() - milli_ref) > timeout_milli)
			{
				return USART_DATA_TIMEOUT;
			}
			//Checks if we have timed out
		}
		//waits for space in the TXE register

		ASM(" strb %1, [%0, #0x4]" ::"r" (usart_object->usart), "r" (*(data++)));
		//put data in data register

	} while(--num_data != 0);
	//run until no more data is left

	return 0;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t UsartGet8Timeout(
	const struct UsartObject * const usart_object,
	uint8_t *data,
	uint32_t num_data,
	const uint32_t timeout_milli)
{
	const volatile USART_TypeDef * usart = usart_object->usart;

	const uint32_t milli_ref = SysTickGetMilli();

	do
	{
		while((usart->SR & USART_SR_RXNE) == 0)
		{
			if((SysTickGetMilli() - milli_ref) > timeout_milli)
			{
				return USART_DATA_TIMEOUT;
			}
			//Checks if we have timed out
		}
		//waits for space in the TXE register

		ASM(" ldrb %0, [%1, #0x4]" :"=r" (*(data++)) : "r" (usart_object->usart));
		//get data from data register

	} while(--num_data != 0);
	//run until no more data is left

	return 0;
}














