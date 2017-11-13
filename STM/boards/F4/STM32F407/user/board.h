//
//
//

#include "stm32f4xx.h"
#include "bare_defines.h"

//########## CLOCKS SETTINGS
#define HSI_SPEED 16000000
#define VCO_MAX 432000000
#define USB_SPEED 48000000
#define CPU_MAX 168000000
#define AHB_MAX 168000000
#define APB1_MAX 42000000
#define APB2_MAX 84000000
//######### END

//########## SPI SETTINGS
//######## SPI1
#define SPI1_TX_DMA_OBJECT &DMA2S3_OBJECT
#define SPI1_TX_DMA_CHANNEL 3
//DMA2S3 Channel 3 & DMA2S5 Channel 3
#define SPI1_RX_DMA_OBJECT &DMA2S2_OBJECT
#define SPI1_RX_DMA_CHANNEL 3
//DMA2S0 Channel 3 & DMA2S2 Channel 3
//######## END

//######## SPI2
#define SPI2_TX_DMA_OBJECT &DMA1S4_OBJECT
#define SPI2_TX_DMA_CHANNEL 0
//DMA1S4 Channel 0
#define SPI2_RX_DMA_OBJECT &DMA1S3_OBJECT
#define SPI2_RX_DMA_CHANNEL 0
//DMA1S3 Channel 0
//######## END

//######## SPI3
#define SPI3_TX_DMA_OBJECT &DMA1S5_OBJECT
#define SPI3_TX_DMA_CHANNEL 0
//DMA1S5 Channel 0 & DMA1S7 Channel 0
#define SPI3_RX_DMA_OBJECT &DMA1S0_OBJECT
#define SPI3_RX_DMA_CHANNEL 0
//DMA1S0 Channel 0 & DMA1S2 Channel 0
//######## END
//########## END SPI BOARD CONSTANTS





#define STARTUP(void) \
do \
{ \
	FlashEnableArt(&FLASH_OBJECT); \
	struct FlashConfig flash_config = {5}; \
	FlashConfig(&FLASH_OBJECT,&flash_config); \
	\
	struct ClockConfig clock_config = CLOCKCONFIG_168MHZ; \
	ClockConfig(&clock_config); \
	\
} while(0)


//Required for bare_timer.h
//#define TIMER_INIT_FUNCTION SysTickEnable
//#define TIMER_UPDATE_FUNCTION SysTickUpdate
//#define TIMER_ISR SysTick_Handler
