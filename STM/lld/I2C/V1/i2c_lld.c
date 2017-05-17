//
//
//
//
//



#include "i2c_lld.h"

#ifdef I2C1
struct I2cObject	I2C1_OBJECT = {{0x40,21,2},0,I2C1};
#endif

#ifdef I2C2
struct I2cObject	I2C2_OBJECT = {{0x40,22,2},0,I2C2};
#endif

#ifdef I2C3
struct I2cObject	I2C3_OBJECT = {{0x40,23,2},0,I2C3};
#endif

#ifdef I2C4
struct I2cObject	I2C4_OBJECT = {{0x40,24,200},0,I2C4};
#endif



uint32_t I2cConfig(
	struct I2cObject *i2c_object,
	struct I2cConfig *i2c_config)
{	
	volatile I2C_TypeDef * const i2c = i2c_object->i2c;

	if((i2c->CR1 & I2C_CR1_PE) != 0)
	//make sure Peripheral is not already used.
	{
		return I2CCONFIG_ENABLED;
		//if not equal to zero. Peripheral is enabled. So already in use.
	}

	uint32_t peripheral_freq = ClockGetPeripheralSpeed(&i2c_object->rcc);
	//get peripheral frequency speed

	{
	const uint32_t clock_period_time = peripheral_freq / i2c_config->baud_rate;

	uint32_t ccr_config = i2c_config->ccr;

	uint32_t rise_time;

	if((ccr_config & I2C_CCR_FS) != 0)
	//clock config for fast mode first
	{
		rise_time = 300;
		//300 nanosecond max rise time for fast mode

		if((ccr_config & I2C_CCR_DUTY) != 0)
		//clock config for fast mode duty cycle = 1 tlow/thigh = 16/9
		{
			ccr_config |= clock_period_time / 25;
			//divide by 25 because tlow * 16 + thigh * 9 = 25
		}
		else
		//duty cycle for fast mode = 0 tlow/thigh = 2
		{
			ccr_config |= clock_period_time / 3;
			//div 3 because 2 * low + high = 3
		}

	}
	else
	//standard mode config if fast mode is not chosen
	{
		rise_time = 1000;
		//1000 nanosecond maximun rise time for standard mode and SMBus mode

		ccr_config |= clock_period_time >> 1;
		//divide by 2 because tlow + thigh = 2
	}
		i2c->CCR = ccr_config;
		i2c->TRISE = rise_time;
		//set inside the brackets because variables are killed
	}
	//compute the ccr register and rise time and then set in brackets so 
	//variables will be killed	

	
	i2c->OAR1 = i2c_config->oar1;
	i2c->OAR2 = i2c_config->oar2;
	i2c->CR2 = i2c_config->cr2 | peripheral_freq / 1000000;
	//turn peripheral freq into mhz for cr2 register

#if defined(I2C_FLTR)
	i2c->FLTR = i2c_config->fltr;
#endif
//only defined for stm32f42 and f43

	i2c->CR1 = i2c_config->cr1 | I2C_CR1_ACK | I2C_CR1_PE;
	//enable Acknowledge everytime. Enable peripheral at end of config always
	//set the rest of the i2c registers

	return 0;
}

uint32_t I2cResetConfig(struct I2cObject *i2c_object)
{
	if((i2c_object->i2c->CR1 & I2C_CR1_PE) == 0)
	//check if peripheral is enabled
	{
		RccResetPeripheral(&i2c_object->rcc);

		return 0;
	}
	//if enabled. We cannot reset

	return I2CCONFIG_ENABLED;
}


uint32_t I2cDisable(struct I2cObject *i2c_object)
{

	volatile I2C_TypeDef * const i2c = i2c_object->i2c;

	if((i2c->SR2 & I2C_SR2_BUSY) != 0)
	//check if peripheral is busy in a communication
	{
		i2c->CR1 &= ~I2C_CR1_PE;

		return 0;
	}

	return I2CDISABLE_TRANSFER;
}




uint32_t I2cStartCondition(
	struct I2cObject *i2c_object, 
	struct I2cTransmissionConfig *i2c_start_config,
	uint32_t timeout)
{
		


}




/*
#define I2C_START_DISABLED 3
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

*/








