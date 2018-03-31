//
//
//
//
//



#ifndef GUI_CHARACTER_H
#define GUI_CHARACTER_H

#include "bareos.h"
#include "font.h"
#include "lcd.h"


struct Window
{
	struct Window *next; //next independent window in the list
	struct Window *prev; //prev independent window in the list
	//independent window list
	
	struct Window *Parent; //children dependent on this window
	struct Window *child; //children dependent on this window

	uint16_t h_offset;
	uint16_t v_offset;
	//location on screen starting at 0,0
	/*
		0,0		...	20,0
		...		...	...
		0,20	...	20,20
	*/
};

struct Screen
{
	struct Lcd *lcd; //lcd reference pointer for the screen.

	void *driver; //driver struct from the lcd header file.
	//The lcd and driver together give the screen its functionality

	uint16_t cursor_x;
	uint16_t cursor_y;
	//where the cursor is in the gui window. actual mouse cursor location

	struct Font *font; //this font overrides the default lcd font.
										 //gui windows can override this font
};





#endif
