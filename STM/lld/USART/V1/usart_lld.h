


#ifndef STM32F767_USART_H
#define STM32F767_USART_H

#include "board.h"
#include "bare_defines.h"
#include "rcc_lld.h"
#include "clock_lld.h"
#include "systick_lld.h"

struct UsartObject
{
	struct RccObject rcc;

	uint16_t unused3;

	volatile USART_TypeDef * const usart;
};

extern struct UsartObject
	USART1_OBJECT, 
	USART2_OBJECT,
	USART3_OBJECT,
	UART4_OBJECT,
	UART5_OBJECT,
	USART6_OBJECT,
	UART7_OBJECT,
	UART8_OBJECT;



struct UsartConfig
{
	union
	{
		struct
		{
			uint32_t baud_rate; //desired baud rate in bits per second (bps)
			uint16_t cr1; //control register 1
			uint16_t cr2; //control register 2
			uint16_t cr3; //control register 3
			uint16_t gtpr; //Guard time and prescaler register
		};
		
		struct
		{
			//LSB
			uint32_t:32; //holder for baud rate value
			//MSB

			//LSB
			uint16_t sbk:1; //send break character enable
			uint16_t rwu:1; //Receiver wakeup. Mute mode
#define RWU_ACTIVE 0
#define RWU_MUTE 1

			uint16_t re:1; //Receiver enable
			uint16_t te:1; //Transmitter enable
			uint16_t idleie:1; //Idle interrupt enable
			uint16_t rxneie:1; //Receiver data ready interrupt enable
			uint16_t tcie:1; //Transfer Complete interrupt enable
			uint16_t txeie:1; //transmitter data empty interrupt enable
			uint16_t peie:1; //Parity error interrupt enable
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

			//LSB
			uint16_t add:4; //Address of Usart node
			uint16_t :1;
			uint16_t lbdl:1; //LIN break detection length
#define LBDL_10BIT 0
#define LBDL_11BIT 1

			uint16_t lbdie:1; //LIN break detection interrupt enable
			uint16_t :1;
			uint16_t lbcl:1; //Last Bit clock output pulse enable
			uint16_t cpha:1; //Clock output Phase
#define CPHA_FIRST 0
#define CPHA_SECOND 1

			uint16_t cpol:1; //Clock output Polarity
#define CPOL_LOW 0
#define CPOL_HIGH 1

			uint16_t clken:1; //Clock output enable
			uint16_t stop:2; //Number of stop bits
#define STOP_1BIT 0
#define STOP_0POINT5BIT 1
#define STOP_2BIT 2
#define STOP_1POINT5BIT 3

			uint16_t linen:1; //LIN mode enable
			uint16_t :1;
			//MSB

			//LSB
			uint16_t eie:1; //Error interrupt enable
			uint16_t iren:1; //IrDA mode enable
			uint16_t irlp:1; //IrDA power mode select
#define IRLP_NORMAL_POWER 0
#define IRLP_LOW_POWER 1

			uint16_t hdsel:1; //Half duplex mode enable
			uint16_t nack:1; //Smartcard NACK enable when parity error happens
			uint16_t scen:1; //Smartcard mode enable
			uint16_t dmar:1; //DMA enable for receiver
			uint16_t dmat:1; //DMA enable for transmitter
			uint16_t rtse:1; //rts enable. Only get data when receive buffer empty
			uint16_t ctse:1; //cts input enable. Only transmit when CTS is low (0)
			uint16_t ctsie:1; //cts interrupt enable
			uint16_t onebit:1; //Sample method for data
			uint16_t :4;
#define ONEBIT_3BIT 0
#define ONEBIT_1BIT 1
			//MSB

			//LSB
			uint16_t psc:8; //IrDA low power clock div. OR Smartcard clock div * 2
											//In normal IrDA mode must be set to 1
			uint16_t gt:8; //SmartCard Gaurd Time in number of baud clock ticks
			//MSB
		};
	};





};


#define USARTCONFIG_ENABLED 1
uint32_t UsartConfig(
	const struct UsartObject * const usart_object,
	const struct UsartConfig * const usart_config);

uint32_t UsartResetConfig(
	const struct UsartObject * const usart_object);

uint32_t UsartDisable(
	const struct UsartObject * const usart_object);

#define USART_DISABLE_TRANSFER 2


#define USART_DATA_TIMEOUT 3

uint32_t UsartPut8Timeout(
	const struct UsartObject * const usart_object,
	const uint8_t *data,
	uint32_t num_data,
	const uint32_t timeout_milli);


uint32_t UsartGet8Timeout(
	const struct UsartObject * const usart_object,
	uint8_t *data,
	uint32_t num_data,
	const uint32_t timeout_milli);




















#endif
