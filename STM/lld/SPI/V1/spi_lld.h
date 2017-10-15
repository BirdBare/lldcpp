//
//
//
//
//



#ifndef SPI_H
#define SPI_H

#include "bare_defines.h"
#include "rcc_lld.h"
#include "clock_lld.h"
#include "systick_lld.h"
#include "communication.h"

struct SpiObject
{
	struct RccObject rcc;

	uint16_t unused2;

	volatile SPI_TypeDef * const spi;
	
	uint32_t clock_frequency; //actual clock frequency. if disabled this is zero.

	void (*interrupt)(struct SpiObject *spi_object); //respective spi is argument
};

extern struct SpiObject
	SPI1_OBJECT,
	SPI2_OBJECT,
	SPI3_OBJECT,
	SPI4_OBJECT,
	SPI5_OBJECT,
	SPI6_OBJECT;

struct SpiConfig2
{
	uint32_t clock_frequency; //spi clock frequency
	
	uint16_t crcpr; //crc polynomial register

	union
	{
		uint16_t cr1; //control register 1

		struct 
		{
			//LSB
			uint16_t cpha:1; //Clock Phase
#define CPHA_FIRST 0
#define CPHA_SECOND 1

			uint16_t cpol:1; //Clock Idle Polarity 
#define CPOL_LOW 0
#define CPOL_HIGH 1

			uint16_t :5; 
			uint16_t lsbfirst:1; //Frame format. MSB vs LSB first
#define LSBFIRST_MSB 0 
#define LSBFIRST_LSB 1

			uint16_t :1; 
			uint16_t ssm:1; //software slave select management enable

			uint16_t :1; 
			uint16_t dff:1; //data frame format. data length
#define DFF_8_BIT 0
#define DFF_16_BIT 1

			uint16_t :1; 
			uint16_t crcen:1; //crc enable bit
			uint16_t bidioe:1; //bidirection mode output enable. transmit or receive
#define BIDIOE_RX 0
#define BIDIOE_TX 1

			uint16_t bidimode:1; //bidirectional mode enable bit
			//MSB
		};
	}; //end cr1 union

	union
	{
		uint16_t cr2; //control register 2

		struct
		{
			//LSB
			uint16_t rxdmaen:1; //Receive Dma Enable bit
			uint16_t txdmaen:1; //Transmit Dma Enable Bit
			uint16_t ssoe:1; //Slave select output enable 
			uint16_t :1; 
			uint16_t frf:1; //frame format. Motorola vs TI
#define FRF_MOTOROLA 0
#define FRF_TI 1
			uint16_t :1; 
			uint16_t rxneie:1; //Receive register not empty interrupt enable bit
			uint16_t txeie:1; //Transfer register empty interrupt enable bit
			uint16_t :8;
			//MSB
		};
	}; //end cr2 union

	struct //fifo threshold register. faster this way and the space was extra
	{
		//LSB
		uint8_t hcd:3; //highest common divisor of number of data to send. can be 1-4
		uint8_t :5; 
		//MSB
	};

	struct //burst length bits. better this way. 0-3 is 1-4-8-16 respectively.
	{			 //burst length = (hcd * 4) / data_size

		uint8_t pburst:2; // calculated above. value 0-3. always 0 if M2P or P2M
		uint8_t mburst:2; // calculated above. value 0-3. 
		uint8_t :4;
	};

};

#define SPICONFIG_ENABLED 1

uint32_t SpiConfig(
	const struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);

uint32_t SpiConfigMaster(
	const struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);





uint32_t SpiResetConfig(
	const struct SpiObject * const spi_object);


#define SPI_DISABLE_TRANSFER 2

uint32_t SpiDisable(
	const struct SpiObject * const spi_object);



uint32_t SpiPut8Blocking(
	const struct SpiObject * const spi_object,
	const struct CommunicationConfig * const communication_config);

uint32_t SpiPut16Blocking(
	const struct SpiObject * const spi_object,
	const struct CommunicationConfig * const communication_config);

uint32_t SpiGet8Blocking(
	const struct SpiObject * const spi_object,
	const struct CommunicationConfig * const communication_config);

uint32_t SpiGet16Blocking(
	const struct SpiObject * const spi_object,
	const struct CommunicationConfig * const communication_config);








#endif








