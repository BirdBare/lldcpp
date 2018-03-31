//
//
//
//
//



#ifndef FONT_H
#define FONT_H

#include "bareos.h"


struct Font
{
	void *character_table;
	
	uint8_t width;
	uint8_t height;
	uint8_t start_character;
	uint8_t end_character;

};







#endif
