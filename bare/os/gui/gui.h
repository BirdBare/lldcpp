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


struct TextBox
{
	uint8_t *text; //null terminated character array

	uint32_t print_position; //c

	uint32_t (*ScrollText)(struct TextBox *box, uint32_t direction);
};




struct Desktop
{
	struct Window *next; 
	struct Window *prev;
	//list of windows on this desktop organized by z index

	struct Font *desktop_font; //this font overrides the screen font	
};










struct Window
{
	struct Window *next; //next independent window in the list
	struct Window *prev; //prev independent window in the list
	//independent window list

	uint16_t h_offset;
	uint16_t v_offset;
	//location on parent starting at 0,0 pixels

	uint16_t width; 
	uint16_t height;
	// width and height in pixels

	struct Font *window_font; //this font overrides the desktop font.

	uint16_t z_index;

	uint16_t repaint_flag;

	union
	{
		struct TextBox textbox;	
	};

	uint32_t (*Draw)(struct Window *Window);
};















struct Screen
{
	struct Lcd *lcd; //lcd user is using for this screen

	void *driver; //driver struct from the lcd header file.
	//The lcd and driver together give the screen its functionality

	uint16_t cursor_x;
	uint16_t cursor_y;
	//where the cursor is in the gui window. actual mouse cursor location

	uint16_t print_x;
	uint16_t print_y;
	//where the printer cursor is on the lcd screen.
	
	struct Font *screen_font; //this font overrides the lcd font.

	struct Window *desktop; //desktop pointer

	uint32_t (*MoveCursor)(struct Screen *screen, uint32_t x, uint32_t y);

};




#endif
