//
//
//
//
//



/* !!!!!!!!!!!!ATTENTION!!!!!!!!!!!!!!!!


    SPI.o MUST BE ADDED TO LIST OF OBJECTS IN MAKEFILE TO USE THIS LIBRARU


    MUST

*/




#ifndef STM32F767_SPI_H
#define STM32F767_SPI_H

#include "BARE_STM32F767.h"
#include "RCC_lld.h"

struct SPIxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile SPI_TypeDef * const SPIx;
};

extern struct SPIxo
	SPI1o,
	SPI2o,
	SPI3o,
	SPI4o,
	SPI5o,
	SPI6o;

#define SPI_CONFIG_ENABLED 1

int SPI_Config(struct SPIxo *SPIo, int CR1, int CR2, int CRCPR);


int SPI_ResetConfig(struct SPIxo *SPIo);


void SPI_Enable(struct SPIxo *SPIo);



#define SPI_DISABLE_TRANSFER 2

int SPI_Disable(struct SPIxo *SPIo);


ALWAYS_INLINE void SPI_Put8(struct SPIxo *SPIo, int Data)
{
	ASM(" strb %1, [%0, #0xc]" : : "r"(SPIo->SPIx), "r" (Data));
}
ALWAYS_INLINE void SPI_Put16(struct SPIxo *SPIo, int Data)
{
	ASM(" strh %1, [%0, #0xc]" : : "r"(SPIo->SPIx), "r" (Data));
}


ALWAYS_INLINE int SPI_Get8(struct SPIxo *SPIo)
{
	uint32_t ret;
	ASM(" ldrb %0, [%1, #0xc]" : "=r" (ret) : "r" (SPIo->SPIx));
	return ret;
}
ALWAYS_INLINE int SPI_Get16(struct SPIxo *SPIo)
{
	uint32_t ret;
	ASM(" ldrh %0, [%1, #0xc]" : "=r" (ret) : "r" (SPIo->SPIx));
	return ret;
}



















//  PROTOTYPES FOR SIMPLE REGISTER FUNCTIONS
//  ALL FORCED INLINE SO THEY DO NOT USE FLASH SPACE
ALWAYS_INLINE void SPI_SetCR1(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE uint32_t SPI_GetCR1(SPI_TypeDef *SPIx);
ALWAYS_INLINE void SPI_SetBitsCR1(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE void SPI_ResetBitsCR1(SPI_TypeDef *SPIx, uint32_t Data);

ALWAYS_INLINE void SPI_SetCR2(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE uint32_t SPI_GetCR2(SPI_TypeDef *SPIx);
ALWAYS_INLINE void SPI_SetBitsCR2(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE void SPI_ResetBitsCR2(SPI_TypeDef *SPIx, uint32_t Data);

ALWAYS_INLINE void SPI_SetSR(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE uint32_t SPI_GetSR(SPI_TypeDef *SPIx);

ALWAYS_INLINE void SPI_SetDR8(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE void SPI_SetDR16(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE uint32_t SPI_GetDR8(SPI_TypeDef *SPIx);
ALWAYS_INLINE uint32_t SPI_GetDR16(SPI_TypeDef *SPIx);

ALWAYS_INLINE void SPI_SetCRCPR(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE uint32_t SPI_GetCRCPR(SPI_TypeDef *SPIx);
ALWAYS_INLINE void SPI_SetBitsCRCPR(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE void SPI_ResetBitsCRCPR(SPI_TypeDef *SPIx, uint32_t Data);

ALWAYS_INLINE uint32_t SPI_GetRXCRCR(SPI_TypeDef *SPIx);

ALWAYS_INLINE uint32_t SPI_GetTXCRCR(SPI_TypeDef *SPIx);

ALWAYS_INLINE void SPI_SetI2SCFGR(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE uint32_t SPI_GetI2SCFGR(SPI_TypeDef *SPIx);
ALWAYS_INLINE void SPI_SetBitsI2SCFGR(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE void SPI_ResetBitsI2SCFGR(SPI_TypeDef *SPIx, uint32_t Data);

ALWAYS_INLINE void SPI_SetI2SPR(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE uint32_t SPI_GetI2SPR(SPI_TypeDef *SPIx);
ALWAYS_INLINE void SPI_SetBitsI2SPR(SPI_TypeDef *SPIx, uint32_t Data);
ALWAYS_INLINE void SPI_ResetBitsI2SPR(SPI_TypeDef *SPIx, uint32_t Data);






#define SPI_CLOCK_SPI1 RCC_APB2ENR_SPI1EN 
#define SPI_CLOCK_SPI4 RCC_APB2ENR_SPI4EN 
#define SPI_CLOCK_SPI5 RCC_APB2ENR_SPI5EN 
#define SPI_CLOCK_SPI6 RCC_APB2ENR_SPI6EN 
/* APB2 */

#define SPI_CLOCK_SPI2 RCC_APB1ENR_SPI2EN  
#define SPI_CLOCK_SPI3 RCC_APB1ENR_SPI3EN
/* APB1 */

#define SPI_EnableClockAPB2(SPI_CLOCK) \
  RCC_EnableClockAPB2(SPI_CLOCK)

#define SPI_EnableClockAPB1(SPI_CLOCK) \
  RCC_EnableClockAPB1(SPI_CLOCK)
  
#define SPI_DisableClockAPB2(SPI_CLOCK) \
  RCC_DisableClockAPB2(SPI_CLOCK)
    
#define SPI_DisableClockAPB1(SPI_CLOCK) \
  RCC_DisableClockAPB1(SPI_CLOCK)


/* SPI CR1 OPTIONS 

  REQUIRED OPTIONS:
      
    SPI_CR1_CLKDIV - CLOCK IS DIVIDED FROM PERIPHERAL CLOCK

*/
#define SPI_CR1_HALFDUPLEX SPI_CR1_BIDIMODE
#define SPI_CR1_HALFDUPLEX_RECEIVE 0
#define SPI_CR1_HALFDUPLEX_TRANSFER SPI_CR1_BIDIOE
//Half Duplex Communication

#define SPI_CR1_CRC SPI_CR1_CRCEN
#define SPI_CR1_CRC_8bit 0
#define SPI_CR1_CRC_16bit SPI_CR1_CRCL
//CRC packet error checking enable and setting set

//#define SPI_CR1_LSBFIRST SPI_CR2_LSBFIRST
//Transfer LSB first instead of MSB

//#define SPI_CR1_MSTR SPI_CR1_MSTR
//#define SPI_CR1_SPE SPI_CR1_SPE
// SPI enable

#define SPI_CR1_CLKDIV_2     0
#define SPI_CR1_CLKDIV_4     SPI_CR1_BR_0
#define SPI_CR1_CLKDIV_8     SPI_CR1_BR_1
#define SPI_CR1_CLKDIV_16    (SPI_CR1_BR_1 | SPI_CR1_BR_0)
#define SPI_CR1_CLKDIV_32    SPI_CR1_BR_2
#define SPI_CR1_CLKDIV_64    (SPI_CR1_BR_2 | SPI_CR1_BR_0)
#define SPI_CR1_CLKDIV_128   (SPI_CR1_BR_2 | SPI_CR1_BR_1)
#define SPI_CR1_CLKDIV_256   (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)
//Baud rate selection from peripheral clock

#define SPI_CR1_MODE_0 (uint16_t) 0b00
#define SPI_CR1_MODE_1 (uint16_t) 0b01
#define SPI_CR1_MODE_2 (uint16_t) 0b10
#define SPI_CR1_MODE_3 (uint16_t) 0b11
//SPI Mode Selection

/* SPI CR2 OPTIONS 

  REQUIRED OPTIONS:

    SPI_CR2_DSIZE - if SPI_CR2_DSIZE !> 8 then add SPI_CR2_FIFO_8bit

*/
#define SPI_CR2_LDMA_TXODD SPI_CR2_LDMA_TX
#define SPI_CR2_LDMA_RXODD SPI_CR2_LDMA_RX
//Number dma data to transfer for data packing

#define SPI_CR2_FIFO_8bit SPI_CR2_FRXTH
//RX fifo size 8 bit or 16bit default

#define SPI_CR2_DSIZE_4 ((uint16_t)0b0011 << 8)
#define SPI_CR2_DSIZE_5 ((uint16_t)0b0100 << 8)
#define SPI_CR2_DSIZE_6 ((uint16_t)0b0101 << 8)
#define SPI_CR2_DSIZE_7 ((uint16_t)0b0110 << 8)
#define SPI_CR2_DSIZE_8 ((uint16_t)0b0111 << 8)
#define SPI_CR2_DSIZE_9 ((uint16_t)0b1000 << 8)
#define SPI_CR2_DSIZE_10 ((uint16_t)0b1001 << 8)
#define SPI_CR2_DSIZE_11 ((uint16_t)0b1010 << 8)
#define SPI_CR2_DSIZE_12 ((uint16_t)0b1011 << 8)
#define SPI_CR2_DSIZE_14 ((uint16_t)0b1100 << 8)
#define SPI_CR2_DSIZE_15 ((uint16_t)0b1110 << 8)
#define SPI_CR2_DSIZE_16 ((uint16_t)0b1111 << 8)
//Data size for SPI

//#define SPI_CR2_TXEIE SPI_CR2_TXEIE //TX empty
//#define SPI_CR2_RXNEIE SPI_CR2_RXNEIE //RX not empty
//#define SPI_CR2_ERRIE SPI_CR2_ERRIE //Error
//Interrupt Enables

#define SPI_CR2_FORMAT_TI
//Data frame format TI or Moto default

#define SPI_CR2_PULSESS SPI_CR2_NSSP
//Pulse SS each data or Pulse SS when SPI on/off/on transition default

//#define SPI_CR2_TXDMAEN SPI_CR2_TXDMAEN
//#define SPI_CR2_RXDMAEN SPI_CR2_RXDMAEN
//Enable DMA REQUESTS


#define SPI_EnableMaster(SPIx, SPI_CR1, SPI_CR2, SPI_CRC_Polynomial) \
do \
{ \
  SPI_SetCRCPR(SPIx,SPI_CRC_Polynomial); \
  SPI_SetCR2(SPIx,(SPI_CR2) | 0b100); \
  SPI_SetI2SCFGR(SPIx, 0); \
  SPI_SetCR1(SPIx,SPI_CR1); \
} while(0)


#define SPI_Send8(SPIx, SPI_DATA) \
SPI_SetDR8(SPIx, SPI_DATA)
  
#define SPI_Receive8(SPIx) \
  SPI_GetDR8(SPIx)

#define SPI_Send16(SPIx, SPI_DATA) \
  SPI_SetDR16(SPIx, SPI_DATA)

#define SPI_Receive16(SPIx) \
  SPI_GetDR16(SPIx)


#define SPI_WaitTransfer(SPIx) \
  PERIPH_WaitTillReset(&(SPIx->SR), SPI_SR_BSY)















//  SPIx->CR1 functions
ALWAYS_INLINE void SPI_SetCR1(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CR1 = Data;
}
ALWAYS_INLINE uint32_t SPI_GetCR1(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldr %0, [%1]" :"=r" (ret) : "r" (SPIx));
  return ret;
}
ALWAYS_INLINE void SPI_SetBitsCR1(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CR1 |= Data;
}
ALWAYS_INLINE void SPI_ResetBitsCR1(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CR1 &= Data;
}


//  SPIx->CR2 functions
ALWAYS_INLINE void SPI_SetCR2(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CR2 = Data;
}
ALWAYS_INLINE uint32_t SPI_GetCR2(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldr %0, [%1, #4]" :"=r" (ret) : "r" (SPIx));
  return ret;
}
ALWAYS_INLINE void SPI_SetBitsCR2(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CR2 |= Data;
}
ALWAYS_INLINE void SPI_ResetBitsCR2(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CR2 &= Data;
}



//  SPIx->SR functions
ALWAYS_INLINE void SPI_SetSR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->SR = Data;
}
ALWAYS_INLINE uint32_t SPI_GetSR(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldr %0, [%1, #8]" :"=r" (ret) : "r" (SPIx));
  return ret;
}



//  SPIx->DR functions
ALWAYS_INLINE void SPI_SetDR8(SPI_TypeDef *SPIx, uint32_t Data)
{
  ASM(" strb %1, [%0, #0xc]" : : "r" (SPIx), "r" (Data));
}
ALWAYS_INLINE void SPI_SetDR16(SPI_TypeDef *SPIx, uint32_t Data)
{
  ASM(" strh %1, [%0, #0xc]" : : "r" (SPIx), "r" (Data));
}
ALWAYS_INLINE uint32_t SPI_GetDR8(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldrb %0, [%1, #0xc]" :"=r" (ret) : "r" (SPIx));
  return ret;
}
ALWAYS_INLINE uint32_t SPI_GetDR16(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldrh %0, [%1, #0xc]" :"=r" (ret) : "r" (SPIx));
  return ret;
}

//  SPIx->CRCPR functions
ALWAYS_INLINE void SPI_SetCRCPR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CRCPR = Data;
}
ALWAYS_INLINE uint32_t SPI_GetCRCPR(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldr %0, [%1, #0x10]" :"=r" (ret) : "r" (SPIx));
  return ret;
}
ALWAYS_INLINE void SPI_SetBitsCRCPR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CRCPR |= Data;
}
ALWAYS_INLINE void SPI_ResetBitsCRCPR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->CRCPR &= Data;
}



//  SPIx->RXCRCR functions
ALWAYS_INLINE uint32_t SPI_GetRXCRCR(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldr %0, [%1, #0x14]" :"=r" (ret) : "r" (SPIx));
  return ret;
}



//  SPIx->TXCRCR functions
ALWAYS_INLINE uint32_t SPI_GetTXCRCR(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldr %0, [%1, #0x18]" :"=r" (ret) : "r" (SPIx));
  return ret;
}




//  SPIx->I2SCFGR functions
ALWAYS_INLINE void SPI_SetI2SCFGR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->I2SCFGR = Data;
}
ALWAYS_INLINE uint32_t SPI_GetI2SCFGR(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldr %0, [%1, #0x1c]" :"=r" (ret) : "r" (SPIx));
  return ret;
}
ALWAYS_INLINE void SPI_SetBitsI2SCFGR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->I2SCFGR |= Data;
}
ALWAYS_INLINE void SPI_ResetBitsI2SCFGR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->I2SCFGR &= Data;
}




//  SPIx->I2SPR functions
ALWAYS_INLINE void SPI_SetI2SPR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->I2SPR = Data;
}
ALWAYS_INLINE uint32_t SPI_GetI2SPR(SPI_TypeDef *SPIx)
{
  uint32_t ret;
  ASM(" ldr %0, [%1, #0x20]" :"=r" (ret) : "r" (SPIx));
  return ret;
}
ALWAYS_INLINE void SPI_SetBitsI2SPR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->I2SPR |= Data;
}
ALWAYS_INLINE void SPI_ResetBitsI2SPR(SPI_TypeDef *SPIx, uint32_t Data)
{
  SPIx->I2SPR &= Data;
}













#endif
