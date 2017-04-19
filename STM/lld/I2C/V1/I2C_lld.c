//
//
//
//
//



#include "I2C_lld.h"
#include "RCC_lld.h"
#include "PERIPH.h"

struct I2Cxo
	I2C1o = {{0x40,21},0,I2C1},
	I2C2o = {{0x40,22},0,I2C2},
	I2C3o = {{0x40,23},0,I2C3},
	I2C4o = {{0x40,24},0,I2C4};

//******************************************************************************
//	
//										 
//	
//******************************************************************************
int I2C_Config(struct I2Cxo *I2Co, int CR1, int OAR1, int OAR2, int TIMINGR, 
	int TIMEOUTR)
{
	volatile I2C_TypeDef * const I2Cx = I2Co->I2Cx;
	
	if((I2Cx->CR1 & I2C_CR1_PE) == 0)
	{
		I2Cx->CR1 = CR1;
		I2Cx->OAR1 = OAR1;
		I2Cx->OAR2 = OAR2;
		I2Cx->TIMINGR = TIMINGR;
		I2Cx->TIMEOUTR = TIMEOUTR;
	
		return 0;
	}
	return I2C_CONFIG_ENABLED;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
int I2C_ResetConfig(struct I2Cxo *I2Co)
{
	if((I2Co->I2Cx->CR1 & I2C_CR1_PE) == 0)
	{
		RCC_Reset((struct RCCxo *)I2Co);

		return 0;
	}

	return I2C_CONFIG_ENABLED;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
void I2C_Enable(struct I2Cxo *I2Co)
{
	I2Co->I2Cx->CR1 |= I2C_CR1_PE;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
int I2C_Disable(struct I2Cxo *I2Co)
{	
	volatile I2C_TypeDef * const I2Cx = I2Co->I2Cx;
	
	if((I2Cx->ISR & I2C_ISR_BUSY) != 0)
	{
		I2Cx->CR1 &= ~I2C_CR1_PE;
		
		return 0;
	}
	return I2C_DISABLE_TRANSFER;
}



#define I2C_ICR_MASK 0b11111100111000

#define I2C_START_DISABLED 3
//******************************************************************************
//	
//										 
//	
//******************************************************************************
int I2C_StartMaster(struct I2Cxo *I2Co, int CR2)
{	
	volatile I2C_TypeDef * const I2Cx = I2Co->I2Cx;

	if((I2Co->I2Cx->CR1 & I2C_CR1_PE) == 0)
		return I2C_START_DISABLED;
	//if I2C is not enabled then return error.

	I2Cx->ICR = I2C_ICR_MASK;
	//reset all flags

	I2Cx->CR2 = CR2 | I2C_CR2_START;

	PERIPH_WaitTillReset(&I2Cx->CR2, I2C_CR2_START);

	if((I2Cx->ISR & I2C_ISR_NACKF) != 0)
		return I2C_START_NACK;
	
	return 0;
}

uint32_t I2C_Start(struct I2Cxo *I2Co)
{
	volatile I2C_TypeDef * const I2Cx = I2Co->I2Cx;

	if((I2Co->I2Cx->CR1 & I2C_CR1_PE) == 0)
		return I2C_START_DISABLED;
	//if I2C is not enabled then return error.

	I2Cx->ICR = I2C_ICR_MASK;
	//reset all flags

	I2Cx->CR2 |= I2C_CR2_START;

	PERIPH_WaitTillReset(&I2Cx->CR2, I2C_CR2_START);

	if((I2Cx->ISR & I2C_ISR_NACKF) != 0)
		return I2C_START_NACK;
	
	return 0;
}



//##############################################################################

void I2C_DIRECT_PUT(struct I2Cxo *I2Co, uint8_t *data)
{
	PERIPH_WaitTillSet(&I2Co->I2Cx->ISR, I2C_ISR_TXE);
	I2C_Put8(I2Co, *data);
}

void I2C_DIRECT_GET(struct I2Cxo *I2Co, uint8_t *data)
{
	PERIPH_WaitTillSet(&I2Co->I2Cx->ISR, I2C_ISR_RXNE);
	*data = I2C_Get8(I2Co);
}

uint32_t I2C_DIRECT_WRITE(struct I2Cxo *I2Co, uint8_t data[], uint32_t num)
{
	uint32_t count = 0;

	volatile uint32_t *ISR = &I2Co->I2Cx->ISR;
	
	do
	{
		PERIPH_WaitTillSet(ISR, I2C_ISR_TXE);
		I2C_Put8(I2Co, data[count++]);
	} while(count < num);

	return count;
}

uint32_t I2C_DIRECT_READ(struct I2Cxo *I2Co, uint8_t data[], uint32_t num)
{
	uint32_t count = 0;

	volatile uint32_t *ISR = &I2Co->I2Cx->ISR;

	do
	{
		PERIPH_WaitTillSet(ISR, I2C_ISR_RXNE);
		data[count++] = I2C_Get8(I2Co); 
	} while(count < num);

	return count;
}










