//
//
//
//
//



#ifndef CLOCK_H
#define CLOCK_H

#include "board.h"
#include "bare_defines.h"
#include "rcc_lld.h"

extern uint32_t CLOCK_SPEED[4];
//Clock Speeds for CPU, AHB, APB1, APB2 clocks in that order.

#define CLOCKCONFIG_168MHZ {.pllcfgr = 16 << 0 | 336 << 6 | 0 << 16 | 7 << 24, \
	.cfgr= SW_PLL << 0 | HPRE_DIV_1 << 4 | PPRE_DIV_4 << 10 | PPRE_DIV_2 << 13}

struct ClockConfig
{
	union
	{
		
		struct //full register definitions
		{
			uint32_t pllcfgr; //for PLL configuration register
			uint32_t cfgr;    //for Clock configuration register
		};
		
		struct
		{
			//LSB //pllcfgr
			uint32_t pllm:6; //Division factor for PLL input clock frequency = VCO
			// SEE REFERENCE MANUAL FOR VALUES AND DESIRED FREQUENCY		

			uint32_t plln:9; //Multiplication factor for VCO
			// SEE REFERENCE MANUAL FOR VALUES AND DESIRED FREQUENCY		

			uint32_t :1;

			uint32_t pllp:2; //Division factor for multiplied VCO
			// SEE REFERENCE MANUAL FOR VALUES AND DESIRED FREQUENCY		

			uint32_t :4;

			uint32_t pllsrc:1; //Main and Audio PLL input clock source
#define PLLSRC_HSI 0
#define PLLSRC_HSE 1

			uint32_t :1;

			uint32_t pllq:4; //Division factor for multiplied VCO for USB, RNG, SDIO
			// SEE REFERENCE MANUAL FOR VALUES AND DESIRED FREQUENCY		

			uint32_t :4;
			//MSB
			
			//LSB //cfgr
			uint32_t sw:2;	//CPU clock source
#define SW_HSI 0
#define SW_HSE 1
#define SW_PLL 2

			uint32_t :2; //status bit. No use
			
			uint32_t hpre:4; //AHB prescaler for CPU clock
#define HPRE_DIV_1 0b0000
#define HPRE_DIV_2 0b1000
#define HPRE_DIV_4 0b1001
#define HPRE_DIV_8 0b1010
#define HPRE_DIV_16 0b1011
#define HPRE_DIV_64 0b1100
#define HPRE_DIV_128 0b1101
#define HPRE_DIV_256 0b1110
#define HPRE_DIV_512 0b1111

			uint32_t :2;
			
			uint32_t ppre1:3; //APB1 prescaler for AHB clock
			uint32_t ppre2:3;	//APB2 prescaler for AHB clock
#define PPRE_DIV_1 0b000
#define PPRE_DIV_2 0b100
#define PPRE_DIV_4 0b101
#define PPRE_DIV_8 0b110
#define PPRE_DIV_16 0b111

			uint32_t rtcpre:5; //HSE division factor for RTC
			// SEE REFERENCE MANUAL FOR VALUES AND DESIRED FREQUENCY		
			

			uint32_t mco1:2; //microcontroller clock output 1
#define MCO1_HSI 0
#define MCO1_LSE 1
#define MCO1_HSE 2
#define MCO1_PLL 3

			uint32_t i2ssrc:1; //Audio clock selection
#define I2SSRC_PLL 0
#define I2SSRC_EXT 1

			uint32_t mco1pre:3; //Microcontroller clock output 1 division factor
			uint32_t mco2pre:3; //Microcontroller clock output 2 division factor
#define MCO_DIV_1 0b000
#define MCO_DIV_2 0b100
#define MCO_DIV_3 0b101
#define MCO_DIV_4 0b110
#define MCO_DIV_5 0b111

			uint32_t mco2:2; //microcontroller clock output 2
#define MCO2_SYS 0
#define MCO2_I2SPLL 1
#define MCO2_HSE 2
#define MCO2_PLL 3
			//MSB
		};
	};
};


// INPUT / PLLM * PLLN = VCO
// VCO / PLLP = CPU clock
// VCO / PLLQ = USB, RNG, SDIO clock

#define PLLM 8
#define PLLN 168
#define PLLP 2

//******************************************************************************
//	
//										 
//	
//******************************************************************************
uint32_t ClockConfig(
const struct ClockConfig * const clock_config);



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t ClockGetPeripheralSpeed(
	const struct RccObject * const rcc_object)
{
	return CLOCK_SPEED[rcc_object->clock_index];
}



//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t ClockGetCpuSpeed(void)
{
	return CLOCK_SPEED[0];
}

ALWAYS_INLINE uint32_t ClockGetAhbSpeed(void)
{
	return CLOCK_SPEED[1];
}

#endif
