//
//
//
//
//


#include "nokia5110_register.h"

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









struct Font FONT_NOKIA5110 = {
	.character_table = NOKIA5110,
	.width = NOKIA5110_WIDTH,
	.height = NOKIA5110_HEIGHT,
	.start_character = NOKIA5110_START,
	.end_character = NOKIA5110_END
};

// LCD DEFINITION //
const struct Lcd NOKIA5110_REGISTER = {
	.size_h = 84,
	.size_v = 48,

	.lcd_font = &FONT_NOKIA5110, 

	.Init = (void *)&Nokia5110RegisterInitLcd,
	.Deinit = (void *)&Nokia5110RegisterResetLcd,
	.Update = (void *)&Nokia5110RegisterUpdateLcd,

	.DisplayOn = (void *)&Nokia5110RegisterDisplayOn,
	.DisplayOff = (void *)&Nokia5110RegisterDisplayOff,
	.DisplayInverse = (void *)&Nokia5110RegisterDisplayInverse,
	.DisplayNormal = (void *)&Nokia5110RegisterDisplayOn,
	
	.BacklightOn = (void *)&Nokia5110RegisterBacklightOn,
	.BacklightOff = (void *)&Nokia5110RegisterBacklightOff,

	.GotoXY = (void *)&Nokia5110RegisterGotoXY,
	.GotoX = (void *)&Nokia5110RegisterGotoX,
	.GotoY = (void *)&Nokia5110RegisterGotoY,

	.Clear = (void *)&Nokia5110Clear,

	.PrintChar = (void *)&Nokia5110PrintChar
};





//############### LCD REGISTER SEND DATA MANIPULATE PINS FUNCTIONS ############

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterSendLcdCommand(
	struct Nokia5110RegisterDriver *driver,
	uint8_t data[],
	uint32_t num_data)
{
	driver->reg.pins &= ~(1 << DATACOMMAND_BIT);

	driver->spi_object->spi_config->interrupt = &Nokia5110Interrupt;

	SpiTransferInterrupt(driver->spi_object,data,data,num_data);

	driver->spi_object->spi_config->interrupt = 0;
		return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterSendLcdData(
	struct Nokia5110RegisterDriver *driver,
	uint8_t data[],
	uint32_t num_data)
{
	driver->reg.pins |= 1 << DATACOMMAND_BIT;

	driver->spi_object->spi_config->interrupt = &Nokia5110Interrupt;

	SpiTransferInterrupt(driver->spi_object,data,data,num_data);

	driver->spi_object->spi_config->interrupt = 0;
	
	return 0;
}



//######################### END SEND DATA MANIPULATE PIN FUNCTIONS ###########


//########################### REQUIRED FUNCTIONS FOR DRIVER ##################

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterBacklightOn(struct Nokia5110RegisterDriver *driver)
{
	Nokia5110RegisterResetPin(driver,1 << LIGHT_BIT);
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterBacklightOff(struct Nokia5110RegisterDriver *driver)
{
	Nokia5110RegisterSetPin(driver,1 << LIGHT_BIT);
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterDisplayOn(struct Nokia5110RegisterDriver *driver)
{
	uint8_t data[2] = {0b00001100};

	Nokia5110RegisterSendLcdCommand(driver,data,1);
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterDisplayInverse(struct Nokia5110RegisterDriver *driver)
{
	uint8_t data[2] = {0b00001101};

	Nokia5110RegisterSendLcdCommand(driver,data,1);
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterDisplayAllOn(struct Nokia5110RegisterDriver *driver)
{
	uint8_t data[2] = {0b00001001};

	Nokia5110RegisterSendLcdCommand(driver,data,1);
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterDisplayOff(struct Nokia5110RegisterDriver *driver)
{
	uint8_t data[2] = {0b00001000};

	Nokia5110RegisterSendLcdCommand(driver,data,1);
	return 0;
}




//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterResetLcd(struct Nokia5110RegisterDriver *driver)
{
	Nokia5110RegisterResetPin(driver, 1 << RESET_BIT);
	Nokia5110RegisterSetPin(driver, 1 << RESET_BIT);
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterUpdateLcd(struct Nokia5110RegisterDriver *driver)
{
	Nokia5110RegisterSetPin(driver, 1 << ENABLE_BIT);
	//set pin

	Nokia5110RegisterResetPin(driver, 1 << ENABLE_BIT);
	//reset pin
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterAdjustScreen(struct Nokia5110RegisterDriver *driver)
{
	uint8_t data[6] = {
		0b00100001, 
		0b10000000 | driver->settings.contrast, 
		0b00010000 | driver->settings.voltage_bias, 
		0b00000100 | driver->settings.temperature_compensation, 
		0b00100000 | driver->settings.addressing_mode};
	
	Nokia5110RegisterSendLcdCommand(driver,data,5);

	return 0;
}


//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterInitLcd(struct Nokia5110RegisterDriver *driver)
{
	if(driver->spi_object == 0 || driver->gpio_object == 0 || 
		driver->gpio_pin == 0)
	{
		return 1;
	}

	Nokia5110RegisterResetLcd(driver);
	Nokia5110RegisterResetLcd(driver);
	//reset

	Nokia5110RegisterUpdateLcd(driver);
	//enable writing

	Nokia5110RegisterAdjustScreen(driver);

	Nokia5110RegisterDisplayOn(driver);
	return 0;
}



//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterGotoXY(struct Nokia5110RegisterDriver *driver, uint32_t x, uint32_t y)
{
	uint8_t data[2] = {0b10000000 | x, 0b01000000 | y};

	Nokia5110RegisterSendLcdCommand(driver,data,2);
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterGotoX(struct Nokia5110RegisterDriver *driver, uint32_t x)
{
	uint8_t data[2] = {0b10000000 | x};

	Nokia5110RegisterSendLcdCommand(driver,data,1);
	return 0;
}

//******************************************************************************
//
//
//
//******************************************************************************
uint32_t Nokia5110RegisterGotoY(struct Nokia5110RegisterDriver *driver, uint32_t y)
{
	uint8_t data[2] = {0b01000000 | y};

	Nokia5110RegisterSendLcdCommand(driver,data,1);
	return 0;
}

//############################ END FUNCTIONS FOR DRIVER ########################








//******************************************************************************
//
//
//
//******************************************************************************
void Nokia5110Interrupt(void *args)
{
	struct Nokia5110RegisterDriver *driver = args;

	if(SpiReceiveReady(driver->spi_object) != 0)
	//make sure the interrupt is called because of tx 
	{	
		Nokia5110RegisterFlashLatchPin(driver);

		SpiGetDataDevice(driver->spi_object);
		//must read data if we are going to use rx interrupt. 
	
		if(++driver->reg.rx_count == 16)
		//count is 16 for rx because tx counts every other clock pulse
		//rx counts every clock pulse
		{
			driver->reg.rx_count = 0;
			driver->reg.rx_data_left = SpiRxDecrementNumData(driver->spi_object);
		}

		if(driver->reg.rx_data_left == 0 && driver->reg.rx_count == 0)
		{
			SpiRxDisableInterrupt(driver->spi_object);
			SpiCallCallback(driver->spi_object);
		}
		//if data left is zero then this will be the last interrupt
		//if driver->count is zero then all the bits on this byte has been sent
		//if clock bit is low then the data has been sent. We can disable
	}

	if(SpiTransmitReady(driver->spi_object) != 0)
	//make sure the interrupt is called because of rx 
	{
		if((driver->reg.pins & (0b1 << CLOCK_BIT)) != 0)
		{
			driver->reg.pins &= ~(0b1 << CLOCK_BIT);
			//reset clock in bit

			if(driver->reg.tx_count == 8)
			{
				driver->reg.tx_count = 0;
				//reset to zero

				if(driver->reg.tx_data_left == 0)
				{
					SpiTxDisableInterrupt(driver->spi_object);
				}
				//if data left is zero then this will be the last interrupt
			}
			//if driver->count equals 8 then we are one last bit and we need more data
		}
		//if clock bit is set then we need to reset it this round
		//we can also do checks here. 
		//if count is equal to 8 then we need to set it to zero to get more data
			//if data_left == 0 then we are all out of everything and we need to quit
		else
		{

			if(driver->reg.tx_count++ == 0)
			{
				driver->reg.data = SpiGetDataObject(driver->spi_object);
				//get data. 

				driver->reg.tx_data_left = SpiTxDecrementNumData(driver->spi_object);
			}
			//everytime the count is equal to 0 we need to get a new data
			//increment count everytime we send data from it.

			if((driver->reg.data & 0b10000000) != 0)
			{
				driver->reg.pins |= 0b1 << DATA_BIT;	
			}
			else
			{
				driver->reg.pins &= ~(0b1 << DATA_BIT);
			}
			//modify the data bit

			driver->reg.data <<= 1;
			//moves bits of data down one

			driver->reg.pins |= 0b1 << CLOCK_BIT;
			//set clock bit high to clock data in.
		}

		SpiPutDataDevice(driver->spi_object,driver->reg.pins);
		//send the data
	}
}


















































