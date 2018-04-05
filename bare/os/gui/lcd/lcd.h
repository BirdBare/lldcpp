//
//
//
//
//



#ifndef lcd_H
#define lcd_H

#include "bareos.h"
#include "font.h"


struct Lcd
{
	uint16_t size_h; //size horizontal in pixels
	uint16_t size_v; //size vertical in pixels
	//size in pixels

	uint16_t pixels_per_line; 

	struct Font *lcd_font; //default font for the screen

	uint32_t (*Init)(void *driver);
	uint32_t (*Deinit)(void *driver);
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

	uint32_t (*Write)(void *driver, uint8_t *data, uint32_t num_data);
	uint32_t (*Read)(void *driver, uint8_t *data, uint32_t num_data);
	//read write pixel functions

	uint32_t (*Clear)(void *driver);

	uint32_t (*PrintChar)(void *driver, uint32_t character, uint32_t size);

};







#endif
