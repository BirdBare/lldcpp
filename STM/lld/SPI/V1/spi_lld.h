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

struct SpiObject
{
	struct RccObject rcc;

	uint16_t unused2;

	volatile SPI_TypeDef * const spi;
};

extern struct SpiObject
	SPI1_OBJECT,
	SPI2_OBJECT,
	SPI3_OBJECT,
	SPI4_OBJECT,
	SPI5_OBJECT,
	SPI6_OBJECT;

struct SpiConfig
{
	union
	{
		struct
		{
			uint16_t cr1; //control register 1
			uint16_t cr2; //control register 2
			uint16_t crcpr; //crc polynomial register
			uint16_t padding;
			uint32_t baud_rate;
		};

		struct
		{
			//LSB
			uint16_t cpha:1; //Clock Phase
#define CPHA_FIRST 0
#define CPHA_SECOND 1

			uint16_t cpol:1; //Clock Idle Polarity 
#define CPOL_LOW 0
#define CPOL_HIGH 1

			uint16_t mstr:1; //Master or Slave selection
#define MSTR_SLAVE 0
#define MSTR_MASTER 1

			uint16_t :3; //baud rate control. Prescaler for spi clock. set in config
#define BR_DIV_2 0b000
#define BR_DIV_4 0b001
#define BR_DIV_8 0b010
#define BR_DIV_16 0b011
#define BR_DIV_32 0b100
#define BR_DIV_64 0b101
#define BR_DIV_128 0b110
#define BR_DIV_256 0b111

			uint16_t :1; //spi enable bit. Not used. Always set in config
			uint16_t lsbfirst:1; //Frame format. MSB vs LSB first
#define LSBFIRST_MSB 0 
#define LSBFIRST_LSB 1

			uint16_t :1; 
			uint16_t ssm:1; //software slave select management
			uint16_t rxonly:1; //receive only bit. Disables Transfers
#define RXONLY_RXTX 0
#define RXONLY_RX 1

			uint16_t dff:1; //data frame format. data length
#define DFF_8_BIT 0
#define DFF_16_BIT 1

			uint16_t :1; //crc send bit. Used in data set 
			uint16_t crcen:1; //crc enable bit
			uint16_t bidioe:1; //bidirection mode output enable. transmit or receive
#define BIDIOE_RX 0
#define BIDIOE_TX 1

			uint16_t bidimode:1; //bidirectional mode enable bit
			//MSB

			//LSB
			uint16_t rxdmaen:1; //Receive Dma Enable bit
			uint16_t txdmaen:1; //Transmit Dma Enable Bit
			uint16_t ssoe:1; //Slave select output enable 
			uint16_t :1; 
			uint16_t frf:1; //frame format. Motorola vs TI
#define FRF_MOTOROLA 0
#define FRF_TI 1
			uint16_t errie:1; //Error interrupt enable bit
			uint16_t rxneie:1; //Receive register not empty interrupt enable bit
			uint16_t txeie:1; //Transfer register empty interrupt enable bit
			uint16_t :8;
			//MSB

			//LSB
			uint16_t :16; //holder for bits in CRC polynomial register
			uint16_t :16; //holder for padding
			uint32_t :32; //holder for baud rate
			//MSB
		};
	};
};

#define SPICONFIG_ENABLED 1

uint32_t SpiConfig(
	const struct SpiObject * const spi_object,
	struct SpiConfig * const spi_config);


uint32_t SpiResetConfig(
	const struct SpiObject * const spi_object);


#define SPI_DISABLE_TRANSFER 2

uint32_t SpiDisable(
	const struct SpiObject * const spi_object);



uint32_t SpiPut8Blocking(
	const struct SpiObject * const spi_object,
	const uint8_t *data,
	uint32_t num_data,
	const uint32_t timeout_milli);

uint32_t SpiPut16Blocking(
	const struct SpiObject * const spi_object,
	const uint16_t *data,
	uint32_t num_data,
	const uint32_t timeout_milli);

uint32_t SpiGet8Blocking(
	const struct SpiObject * const spi_object,
	uint8_t *data,
	uint32_t num_data,
	const uint32_t timeout_milli);

uint32_t SpiGet16Blocking(
	const struct SpiObject * const spi_object,
	uint16_t *data,
	uint32_t num_data,
	const uint32_t timeout_milli);








#endif








