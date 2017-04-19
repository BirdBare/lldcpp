//
//
//
//
//



#ifndef STM32F767_I2C_H
#define STM32F767_I2C_H

#include "board.h"
#include "BARE_DEFINES.h"
#include "RCC_lld.h"
#include "MUTEX.h"
#include "BUFFER.h"
#include "STREAM.h"

struct I2Cxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile I2C_TypeDef * const I2Cx;

	struct STREAM_BASE_FUNCTIONS functions;

	struct MUTEX mutex;

	struct BUFFER txbuffer;
	struct BUFFER rxbuffer;

};

extern struct I2Cxo
	I2C1o,
	I2C2o,
	I2C3o,
	I2C4o;

#define I2C_CONFIG_ENABLED 1

//******************************************************************************
//	
//										 
//	
//******************************************************************************
int I2C_Config(struct I2Cxo *I2Co, int CR1, int OAR1, int OAR2, int TIMINGR, 
	int TIMEOUTR);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
int I2C_ResetConfig(struct I2Cxo *I2Co);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void I2C_Enable(struct I2Cxo *I2Co);

#define I2C_DISABLE_TRANSFER 2
//******************************************************************************
//	
//										 
//	
//******************************************************************************
int I2C_Disable(struct I2Cxo *I2Co);

#define I2C_START_NACK 1
//******************************************************************************
//	
//										 
//	
//******************************************************************************
int I2C_StartMaster(struct I2Cxo *I2Co, int CR2);

//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE uint32_t I2C_Get8(struct I2Cxo *I2Co)
{
	uint32_t ret;
	ASM(" ldrb %0, [%1, #0x24]" :"=r" (ret) : "r" (I2Co->I2Cx));
	return ret;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void I2C_Put8(struct I2Cxo *I2Co, uint32_t Data)
{
	ASM(" strb %1, [%0, #0x28]" ::"r" (I2Co->I2Cx), "r" (Data));
}









//******************************************************************************
//	
//										 
//	
//******************************************************************************
static uint32_t I2C_CalculateTimingReg(uint32_t FreqHz)
{
  uint32_t timing = 48000000 / FreqHz;

  uint32_t prescale = 1;
  do
  {
    prescale++;
  }
  while(timing/prescale > 510);

  timing /= prescale;

  timing -= 25;
  
  timing >>= 1;

  return( timing | timing << 8 | (prescale-2) << 28); 
}

#define GetPreScaler(FreqHz) (((48000000 / (FreqHz)) / 510) + 1)
#define GetTiming(FreqHz) \
  ((((48000000/ (FreqHz)) / GetPreScaler(FreqHz)) - 25 ) >> 1)




#define I2C_CR2_WRITE  0
#define I2C_CR2_READ  I2C_CR2_RD_WRN


#endif
