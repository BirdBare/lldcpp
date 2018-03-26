//
//
//
//
//



#ifndef lcd_H
#define lcd_H

#include "bareos.h"


struct Lcd
{
	uint16_t size_h; //size horizontal in pixels
	uint16_t size_v; //size vertical in lines

	uint32_t (*Reset)(void *driver);
	uint32_t (*Init)(void *driver);
	uint32_t (*Update)(void *driver);
	//basic functions

	uint32_t (*DisplayOn)(void *driver);
	uint32_t (*DisplayOff)(void *driver);
	uint32_t (*DisplayNormal)(void *driver);
	uint32_t (*DisplayInverse)(void *driver);
	//display functions

	uint32_t (*BacklightOn)(void *driver);
	uint32_t (*BacklightDim)(void *driver, uint32_t percent);
	uint32_t (*BacklightOff)(void *driver);
	//backlight functions

	uint32_t (*GotoXY)(void *driver, uint32_t x, uint32_t y);
	uint32_t (*GotoX)(void *driver, uint32_t x);
	uint32_t (*GotoY)(void *driver, uint32_t y);
	//move cursor function

	uint32_t (*WritePixel)(void *driver, uint8_t *data, uint32_t num_data);
	uint32_t (*ReadPixel)(void *driver, uint8_t *data, uint32_t num_data);
	uint32_t (*WritePixelArea)(void *driver, uint8_t *data, 
		uint32_t width, uint32_t height);
	uint32_t (*ReadPixelArea)(void *driver, uint8_t *data, 
		uint32_t width, uint32_t height);
	//read write pixel functions

};



struct Screen
{
	struct Lcd *lcd;
	
	void *driver;

};







#endif
