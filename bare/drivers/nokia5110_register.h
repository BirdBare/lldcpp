//
//
//
//
//

#ifndef NOKIA5110_REGISTER_H
#define NOKIA5110_REGISTER_H

#include "gpio_hal.h"
#include "spi_hal.h"
#include "gui_character.h"


#define RESET_BIT 2
#define ENABLE_BIT 3
#define DATACOMMAND_BIT 4
#define DATA_BIT 5
#define CLOCK_BIT 6
#define LIGHT_BIT 7



/*  PINS for controller to register to lcd      PINS for controller to LCD
     pins 0 & 1 can be used as outputs.          
               sn74hc959                       RST   -*1*- RESETBIT     
               1 *-|-* +                       CE    -*2*- ENABLEBIT       
      RESETBIT 2 *-|-* 0                       DC    -*3*- DATACOMMANDBIT
     ENABLEBIT 3 *-|-* RDATAPIN                DIN   -*4*- DATABIT
DATACOMMANDBIT 4 *-|-* -                       CLK   -*5*- CLOCKBIT
       DATABIT 5 *-|-* RLATCHPIN               VCC   -*6*- + 
      CLOCKBIT 6 *-|-* RCLOCKPIN               LIGHT -*7*- LIGHTBIT
      LIGHTBIT 7 *-|-* +                       GND   -*8*- - 
               - *-|-* dataout
*/


#define DEFAULT_5X1_NUM 95
#define DEFAULT_5X1_WIDTH 5
#define DEFAULT_5X1_HEIGHT 1
#define DEFAULT_5X1_START ' '
#define DEFAULT_5X1_END '~'

//Characters. 8 bits high and 5 bits wide.
static uint8_t DEFAULT_5X1[DEFAULT_5X1_NUM][DEFAULT_5X1_WIDTH] =
{
   { 0x00, 0x00, 0x00, 0x00, 0x00 } // 20 (space)
	,{ 0x00, 0x00, 0x5f, 0x00, 0x00 } // 21 !
	,{ 0x00, 0x07, 0x00, 0x07, 0x00 } // 22 "
	,{ 0x14, 0x7f, 0x14, 0x7f, 0x14 } // 23 #
	,{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 } // 24 $
	,{ 0x23, 0x13, 0x08, 0x64, 0x62 } // 25 %
	,{ 0x36, 0x49, 0x55, 0x22, 0x50 } // 26 &
	,{ 0x00, 0x05, 0x03, 0x00, 0x00 } // 27 '
	,{ 0x00, 0x1c, 0x22, 0x41, 0x00 } // 28 (
	,{ 0x00, 0x41, 0x22, 0x1c, 0x00 } // 29 )
	,{ 0x14, 0x08, 0x3e, 0x08, 0x14 } // 2a *
	,{ 0x08, 0x08, 0x3e, 0x08, 0x08 } // 2b +
	,{ 0x00, 0x50, 0x30, 0x00, 0x00 } // 2c ,
	,{ 0x08, 0x08, 0x08, 0x08, 0x08 } // 2d -
	,{ 0x00, 0x60, 0x60, 0x00, 0x00 } // 2e .
	,{ 0x20, 0x10, 0x08, 0x04, 0x02 } // 2f /
	,{ 0x3e, 0x51, 0x49, 0x45, 0x3e } // 30 0
	,{ 0x00, 0x42, 0x7f, 0x40, 0x00 } // 31 1
	,{ 0x42, 0x61, 0x51, 0x49, 0x46 } // 32 2
	,{ 0x21, 0x41, 0x45, 0x4b, 0x31 } // 33 3
	,{ 0x18, 0x14, 0x12, 0x7f, 0x10 } // 34 4
	,{ 0x27, 0x45, 0x45, 0x45, 0x39 } // 35 5
	,{ 0x3c, 0x4a, 0x49, 0x49, 0x30 } // 36 6
	,{ 0x01, 0x71, 0x09, 0x05, 0x03 } // 37 7
	,{ 0x36, 0x49, 0x49, 0x49, 0x36 } // 38 8
	,{ 0x06, 0x49, 0x49, 0x29, 0x1e } // 39 9
	,{ 0x00, 0x36, 0x36, 0x00, 0x00 } // 3a :
	,{ 0x00, 0x56, 0x36, 0x00, 0x00 } // 3b ;
	,{ 0x08, 0x14, 0x22, 0x41, 0x00 } // 3c <
	,{ 0x14, 0x14, 0x14, 0x14, 0x14 } // 3d =
	,{ 0x00, 0x41, 0x22, 0x14, 0x08 } // 3e >
	,{ 0x02, 0x01, 0x51, 0x09, 0x06 } // 3f ?
	,{ 0x32, 0x49, 0x79, 0x41, 0x3e } // 40 @
	,{ 0x7e, 0x11, 0x11, 0x11, 0x7e } // 41 A
	,{ 0x7f, 0x49, 0x49, 0x49, 0x36 } // 42 B
	,{ 0x3e, 0x41, 0x41, 0x41, 0x22 } // 43 C
	,{ 0x7f, 0x41, 0x41, 0x22, 0x1c } // 44 D
	,{ 0x7f, 0x49, 0x49, 0x49, 0x41 } // 45 E
	,{ 0x7f, 0x09, 0x09, 0x09, 0x01 } // 46 F
	,{ 0x3e, 0x41, 0x49, 0x49, 0x7a } // 47 G
	,{ 0x7f, 0x08, 0x08, 0x08, 0x7f } // 48 H
	,{ 0x00, 0x41, 0x7f, 0x41, 0x00 } // 49 I
	,{ 0x20, 0x40, 0x41, 0x3f, 0x01 } // 4a J
	,{ 0x7f, 0x08, 0x14, 0x22, 0x41 } // 4b K
	,{ 0x7f, 0x40, 0x40, 0x40, 0x40 } // 4c L
	,{ 0x7f, 0x02, 0x0c, 0x02, 0x7f } // 4d M
	,{ 0x7f, 0x04, 0x08, 0x10, 0x7f } // 4e N
	,{ 0x3e, 0x41, 0x41, 0x41, 0x3e } // 4f O
	,{ 0x7f, 0x09, 0x09, 0x09, 0x06 } // 50 P
	,{ 0x3e, 0x41, 0x51, 0x21, 0x5e } // 51 Q
	,{ 0x7f, 0x09, 0x19, 0x29, 0x46 } // 52 R
	,{ 0x46, 0x49, 0x49, 0x49, 0x31 } // 53 S
	,{ 0x01, 0x01, 0x7f, 0x01, 0x01 } // 54 T
	,{ 0x3f, 0x40, 0x40, 0x40, 0x3f } // 55 U
	,{ 0x1f, 0x20, 0x40, 0x20, 0x1f } // 56 V
	,{ 0x3f, 0x40, 0x38, 0x40, 0x3f } // 57 W
	,{ 0x63, 0x14, 0x08, 0x14, 0x63 } // 58 X
	,{ 0x07, 0x08, 0x70, 0x08, 0x07 } // 59 Y
	,{ 0x61, 0x51, 0x49, 0x45, 0x43 } // 5a Z
	,{ 0x00, 0x7f, 0x41, 0x41, 0x00 } // 5b [
	,{ 0x02, 0x04, 0x08, 0x10, 0x20 } // 5c backslash
	,{ 0x00, 0x41, 0x41, 0x7f, 0x00 } // 5d ]
	,{ 0x04, 0x02, 0x01, 0x02, 0x04 } // 5e ^
	,{ 0x40, 0x40, 0x40, 0x40, 0x40 } // 5f _
	,{ 0x00, 0x01, 0x02, 0x04, 0x00 } // 60 `
	,{ 0x20, 0x54, 0x54, 0x54, 0x78 } // 61 a
	,{ 0x7f, 0x48, 0x44, 0x44, 0x38 } // 62 b
	,{ 0x38, 0x44, 0x44, 0x44, 0x20 } // 63 c
	,{ 0x38, 0x44, 0x44, 0x48, 0x7f } // 64 d
	,{ 0x38, 0x54, 0x54, 0x54, 0x18 } // 65 e
	,{ 0x08, 0x7e, 0x09, 0x01, 0x02 } // 66 f
	,{ 0x0c, 0x52, 0x52, 0x52, 0x3e } // 67 g
	,{ 0x7f, 0x08, 0x04, 0x04, 0x78 } // 68 h
	,{ 0x00, 0x44, 0x7d, 0x40, 0x00 } // 69 i
	,{ 0x20, 0x40, 0x44, 0x3d, 0x00 } // 6a j 
	,{ 0x7f, 0x10, 0x28, 0x44, 0x00 } // 6b k
	,{ 0x00, 0x41, 0x7f, 0x40, 0x00 } // 6c l
	,{ 0x7c, 0x04, 0x18, 0x04, 0x78 } // 6d m
	,{ 0x7c, 0x08, 0x04, 0x04, 0x78 } // 6e n
	,{ 0x38, 0x44, 0x44, 0x44, 0x38 } // 6f o
	,{ 0x7c, 0x14, 0x14, 0x14, 0x08 } // 70 p
	,{ 0x08, 0x14, 0x14, 0x18, 0x7c } // 71 q
	,{ 0x7c, 0x08, 0x04, 0x04, 0x08 } // 72 r
	,{ 0x48, 0x54, 0x54, 0x54, 0x20 } // 73 s
	,{ 0x04, 0x3f, 0x44, 0x40, 0x20 } // 74 t
	,{ 0x3c, 0x40, 0x40, 0x20, 0x7c } // 75 u
	,{ 0x1c, 0x20, 0x40, 0x20, 0x1c } // 76 v
	,{ 0x3c, 0x40, 0x30, 0x40, 0x3c } // 77 w
	,{ 0x44, 0x28, 0x10, 0x28, 0x44 } // 78 x
	,{ 0x0c, 0x50, 0x50, 0x50, 0x3c } // 79 y
	,{ 0x44, 0x64, 0x54, 0x4c, 0x44 } // 7a z
	,{ 0x00, 0x08, 0x36, 0x41, 0x00 } // 7b {
	,{ 0x00, 0x00, 0x7f, 0x00, 0x00 } // 7c |
	,{ 0x00, 0x41, 0x36, 0x08, 0x00 } // 7d }
	,{ 0x10, 0x08, 0x08, 0x10, 0x08 } // 7e ~
};










extern struct Font FONT_DEFAULT_5X1;

extern const struct Lcd NOKIA5110_REGISTER;

// LCD Driver Definition
struct Nokia5110RegisterDriver
{
	struct SpiObject *spi_object; //spi object driving the lcd

	struct GpioObject *gpio_object;

	uint32_t gpio_pin;

	struct
	{
		uint8_t contrast;
		uint8_t voltage_bias;
		uint8_t temperature_compensation;
		uint8_t addressing_mode;
	} settings;

	//#####DO NOT SET ANYTHING BEYOND HERE. ALL ZEROS ###########

	uint16_t cursor_x;
	uint16_t cursor_y;

	volatile struct 
	{
		uint8_t pins; //values seen on the nokia pins from RST:2 - LIGHT:7

		uint8_t data; //for use by driver function //data we are bit counting on

		uint8_t tx_count; //for use by driver function //count of bit we are on in data
		uint8_t rx_count; //for use by driver function //count of bit we are on in data

		uint16_t tx_data_left;
		uint16_t rx_data_left;
	} reg;
};


//############### LCD REGISTER SEND DATA MANIPULATE PINS FUNCTIONS ############

void Nokia5110Interrupt(void *args);

//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t Nokia5110RegisterFlashLatchPin(
	struct Nokia5110RegisterDriver *driver)
{
		LldGpioSetOutput(driver->gpio_object, driver->gpio_pin);
			//set data enable pin

		LldGpioResetOutput(driver->gpio_object, driver->gpio_pin);
			//disable data enable pin	

		return 0;
}


//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t Nokia5110RegisterSendRegister(
	struct Nokia5110RegisterDriver *driver,
	uint32_t *data)
{
	uint32_t trash;

	SpiTransferInterrupt(driver->spi_object,data,&trash,1);
		return 0;
}



//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterSendLcdCommand(
	struct Nokia5110RegisterDriver *driver,
	uint8_t data[],
	uint32_t num_data);


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterSendLcdData(
	struct Nokia5110RegisterDriver *driver,
	uint8_t data[],
	uint32_t num_data);


//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t Nokia5110RegisterSetPin(struct Nokia5110RegisterDriver *driver, uint32_t bit)
{
	uint32_t pin = driver->reg.pins = bit | driver->reg.pins;

	Nokia5110RegisterSendRegister(driver,&pin);
	
	Nokia5110RegisterFlashLatchPin(driver);
		return 0;
}



//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t Nokia5110RegisterResetPin(struct Nokia5110RegisterDriver *driver, uint32_t bit)
{
	uint32_t pin = driver->reg.pins = ~bit & driver->reg.pins;

	Nokia5110RegisterSendRegister(driver,&pin);

	Nokia5110RegisterFlashLatchPin(driver);
		return 0;
}



//******************************************************************************
//
//
//
//******************************************************************************
static inline uint32_t Nokia5110RegisterTogglePin(struct Nokia5110RegisterDriver *driver, uint32_t bit)
{
	uint32_t pin = driver->reg.pins = bit ^ driver->reg.pins;

	Nokia5110RegisterSendRegister(driver,&pin);

	Nokia5110RegisterFlashLatchPin(driver);
		return 0;
}

//######################### END SEND DATA MANIPULATE PIN FUNCTIONS ###########


//########################### REQUIRED FUNCTIONS FOR DRIVER ##################

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterBacklightOn(struct Nokia5110RegisterDriver *driver);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterBacklightOff(struct Nokia5110RegisterDriver *driver);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterDisplayOn(struct Nokia5110RegisterDriver *driver);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterDisplayInverse(struct Nokia5110RegisterDriver *driver);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterDisplayAllOn(struct Nokia5110RegisterDriver *driver);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterDisplayOff(struct Nokia5110RegisterDriver *driver);




//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterResetLcd(struct Nokia5110RegisterDriver *driver);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterUpdateLcd(struct Nokia5110RegisterDriver *driver);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterAdjustScreen(struct Nokia5110RegisterDriver *driver);


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterInitLcd(struct Nokia5110RegisterDriver *driver);



//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterGotoXY(struct Nokia5110RegisterDriver *driver, 
	uint32_t x, uint32_t y);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterGotoX(struct Nokia5110RegisterDriver *driver, 
	uint32_t x);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterGotoY(struct Nokia5110RegisterDriver *driver, 
	uint32_t y);

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterWriteLineArea(
	struct Nokia5110RegisterDriver *driver,
	uint8_t *data, uint32_t width, uint32_t height);
//############################ END FUNCTIONS FOR DRIVER ########################




















































//******************************************************************************
//
//
//
//******************************************************************************
static void Nokia5110Print(struct Nokia5110RegisterDriver *driver, uint32_t data, 
	uint32_t width, uint32_t height)
{
	uint8_t datas[width];
	uint32_t counter = 0;

	do
	{
		datas[counter] = data;

	}while(++counter < width);

	counter = 0;

	do
	{
		Nokia5110RegisterSendLcdData(driver,datas,width);

	} while(++counter < height);

}


//******************************************************************************
//
//
//
//******************************************************************************
static void Nokia5110PrintBitmap(struct Nokia5110RegisterDriver *driver, 
	uint8_t *map, uint32_t width, uint32_t height)
{
	Nokia5110RegisterSendLcdData(driver,map,width*height);
}

//******************************************************************************
//
//
//
//******************************************************************************
static void Nokia5110PrintChar(struct Nokia5110RegisterDriver *driver, const uint8_t character)
{
	Nokia5110RegisterSendLcdData(driver,DEFAULT_5X1[character-DEFAULT_5X1_START],
		DEFAULT_5X1_WIDTH);
}

//******************************************************************************
//
//
//
//******************************************************************************
static void Nokia5110PrintCharUnderlined(struct Nokia5110RegisterDriver *driver, const uint8_t character)
{
	uint8_t data[DEFAULT_5X1_WIDTH];
	uint32_t count = 0;

	do
	{
		data[count] = DEFAULT_5X1[character-DEFAULT_5X1_START][count] | 0b1000000;

	}while(++count < DEFAULT_5X1_WIDTH);
		

	Nokia5110RegisterSendLcdData(driver,data,DEFAULT_5X1_WIDTH);
}

//******************************************************************************
//
//
//
//******************************************************************************
static void Nokia5110PrintCharTable(struct Nokia5110RegisterDriver *driver)
{
	uint32_t character = DEFAULT_5X1_START;

	do
	{
		Nokia5110PrintChar(driver,character);
	} while(++character < DEFAULT_5X1_END);

}

//******************************************************************************
//
//
//
//******************************************************************************
static void Nokia5110PrintString(struct Nokia5110RegisterDriver *driver,
	const char *string)
{
	uint32_t counter = 0;

	do
	{
		Nokia5110PrintChar(driver,(uint8_t)string[counter]);
	} while(string[++counter] != '\0');
}

static void Nokia5110PrintNumber(struct Nokia5110RegisterDriver *driver, uint32_t number)
{
	uint32_t counter = 0;
	uint8_t holder[12];

	while(number > 0)
	{
		holder[counter++] = number % 10;
		number /= 10;
	}

	if(counter == 0)
	{
		Nokia5110PrintChar(driver, '0');
	}
	else
	{
		do
		{
			Nokia5110PrintChar(driver, holder[--counter] + '0');
		} while(counter > 0);
	}
}



#endif
