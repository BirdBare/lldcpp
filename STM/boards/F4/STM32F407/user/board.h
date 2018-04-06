//
//
//

#include "stm32f4xx.h"
#include "bare_defines.h"

#define USE_BAREOS

//########## CLOCKS SETTINGS
#define HSI_SPEED 16000000
#define VCO_MAX 432000000
#define VCO_MIN 100000000
#define USB_SPEED 48000000
#define CPU_MAX 168000000
#define AHB_MAX 168000000
#define APB1_MAX 42000000
#define APB2_MAX 84000000
//######### END

//########## SPI SETTINGS
//######## SPI1
#define SPI1_TX_DMA_HAL &DMA2S3_HAL
#define SPI1_TX_DMA_CHANNEL 3
//DMA2S3 Channel 3 & DMA2S5 Channel 3
#define SPI1_RX_DMA_HAL &DMA2S2_HAL
#define SPI1_RX_DMA_CHANNEL 3
//DMA2S0 Channel 3 & DMA2S2 Channel 3
//######## END

//######## SPI2
#define SPI2_TX_DMA_HAL &DMA1S4_HAL
#define SPI2_TX_DMA_CHANNEL 0
//DMA1S4 Channel 0
#define SPI2_RX_DMA_HAL &DMA1S3_HAL
#define SPI2_RX_DMA_CHANNEL 0
//DMA1S3 Channel 0
//######## END

//######## SPI3
#define SPI3_TX_DMA_HAL &DMA1S5_HAL
#define SPI3_TX_DMA_CHANNEL 0
//DMA1S5 Channel 0 & DMA1S7 Channel 0
#define SPI3_RX_DMA_HAL &DMA1S0_HAL
#define SPI3_RX_DMA_CHANNEL 0
//DMA1S0 Channel 0 & DMA1S2 Channel 0
//######## END
//########## END SPI BOARD CONSTANTS



