//
//
//
//
//



#ifndef STM32F767_I2C_H
#define STM32F767_I2C_H

#include "board.h"
#include "bare_defines.h"
#include "rcc_lld.h"
#include "clock_lld.h"

struct I2cObject
{
	struct RccObject rcc;

	uint16_t unused2;

	volatile I2C_TypeDef * const i2c;
};

extern struct I2cObject
	I2C1_OBJECT,
	I2C2_OBJECT,
	I2C3_OBJECT,
	I2C4_OBJECT;


struct I2cConfig
{
	union
	{
		struct
		{
			uint32_t baud_rate; //clock rate of the I2C bus
			uint16_t cr1; //control register 1
			uint16_t cr2; //control register 2
			uint16_t oar1; //own address register 1. first I2c address
			uint16_t oar2; //own address register 2. Second I2c address
			uint16_t ccr; //clock control register
			uint16_t fltr; //analog digital filter register
		};

		struct
		{
			//LSB
			uint32_t :32; //baud rate 
			//MSB

			//LSB
			uint16_t :1; //Peripheral Enable bit. Set in Config
			uint16_t smbus:1;
			uint16_t :1;
			uint16_t smbtype:1;
			uint16_t enarp:1;
			uint16_t enpec:1;
			uint16_t engc:1; 
			uint16_t nostretch:1;
			uint16_t :1; //start bit. used in i2c send
			uint16_t :1; //stop bit. used in i2c stop
			uint16_t :1; //Acknowledge bit. Used in i2c send and receiver
			uint16_t pos:1; //position of pec data
			uint16_t pec:1; // packet error checking enable bit
			uint16_t alert:1; //SMBus alert bit
			uint16_t :1;
			uint16_t :1; //software reset bit. Not Used.
			//MSB

			//LSB
			uint16_t :6; //Frequency of Apb bus bits. Set in config  
			uint16_t :2;
			uint16_t iterren:1; //Bus error interrupt enable bit
			uint16_t itevten:1; //bus event interrupt enable bit
			uint16_t itbufen:1; //RX TX interrupt enable bit
			uint16_t dmaen:1; //dma enable bit
			uint16_t last:1; //dma EOT is the last transfer enable bit.
			uint16_t :3;
			//MSB
			
			//LSB
			union
			{
				struct
				{
					uint16_t :1;
					uint16_t add71:7; //7 bit slave address	
					uint16_t :2;
				};

				struct
				{
					uint16_t add10:10; //10 bit slave addres
				};
			};
			uint16_t :5;
			uint16_t addmode:1; //slave address mode
			//MSB

			//LSB
			uint16_t endual:1; //enable dual addressing mode
			uint16_t add72:7;
			uint16_t :8;
			//MSB

			//LSB
			uint16_t :12; //clock control bits calculated in config
			uint16_t :2;
			uint16_t duty:1; //fast mode duty cycle
			uint16_t fs:1; //fast mode vs standard mode selection
			//MSB

			//MSB
			uint16_t dnf:4;	//digital noise filter bits
			uint16_t anoff:1; //analog noise filter disable bits
			uint16_t :11;
			//LSB
		};
	};
};



#define I2CCONFIG_ENABLED 1
uint32_t I2cConfig(
	struct I2cObject *i2c_object, 
	struct I2cConfig *i2c_config);



uint32_t I2cResetConfig(struct I2cObject *i2c_object);


#define I2CDISABLE_TRANSFER 2
uint32_t I2cDisable(struct I2cObject *i2c_object);



struct I2cTransmissionConfig
{
	uint16_t padding1;

	union
	{
		struct
		{
			union
			{
				struct
				{
					uint8_t padding2; 
					uint8_t slave_address_rw; //complete 8 bit slave addres with rw bit
				};

				struct
				{
					//LSB
					uint8_t :8; //padding holder
					uint8_t direction:1; //direction bit. Read or Write
#define DIRECTION_READ 1
#define DIRECTION_WRITE 0

					uint8_t slave_address:7; //7 bit slave address bits
					//MSB
				};
			};
		} bit7; //bit 7 struct is used for 7 bit addressing

		struct
		{
			union
			{
				struct
				{
					uint16_t slave_address_rw; //complete 10 bit slave address with 
																				 //header and rw bit
				};

				struct
				{
					uint8_t slave_address_rw_high; //header + 2 address bits + rw bit
					uint8_t slave_address_rw_low;	 //last 8 bits of slave address
				};

				struct
				{
					//LSB
					uint8_t direction:1; //direction bit. Read or Write
#define DIRECTION_READ 1
#define DIRECTION_WRITE 0
					
					uint8_t slave_address_high:7; //header plus first 2 bits of address
					uint8_t slave_address_low; //last 8 bits of slave address
					//MSB
				};
			};
		} bit10; //bit 10 struct is used for 10 bit addressing

	};
};



/*
int I2C_Disable(struct I2Cxo *I2Co);

#define I2C_START_NACK 1
int I2C_StartMaster(struct I2Cxo *I2Co, int CR2);


ALWAYS_INLINE uint32_t I2C_Get8(struct I2Cxo *I2Co)
{
	uint32_t ret;
	ASM(" ldrb %0, [%1, #0x24]" :"=r" (ret) : "r" (I2Co->I2Cx));
	return ret;
}

ALWAYS_INLINE void I2C_Put8(struct I2Cxo *I2Co, uint32_t Data)
{
	ASM(" strb %1, [%0, #0x28]" ::"r" (I2Co->I2Cx), "r" (Data));
}
*/








#endif
