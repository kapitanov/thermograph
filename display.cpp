#include "Arduino.h"
#include "_config.h"
#include "display.h"
#include "log.h"

using namespace thermograph;

display_t thermograph::display;

const int display_t::CHAR_WIDTH = 5;

display_t::display_t()
	: _lcd(LCD_RS_PORT, LCD_ENABLE_PORT, LCD_D0_PORT, LCD_D1_PORT, LCD_D2_PORT, LCD_D3_PORT),
	_bitmap(&_lcd, LCD_BITMAP_X, LCD_BITMAP_Y)
{ }

void display_t::init()
{
	log.debug(F("display\tinit"));

	_lcd.begin(LCD_WIDTH, LCD_HEIGHT);
	_bitmap.begin();
	_bitmap.move(12, 0);

	log.debug(F("display\tdone"));
}

custom_char display_t::to_custom_char(int x)
{
	custom_char c = CHAR_QUESTION;
	switch (x)
	{
	case 0:
		c = CHAR_0;
		break;
	case 1:
		c = CHAR_1;
		break;
	case 2:
		c = CHAR_2;
		break;
	case 3:
		c = CHAR_3;
		break;
	case 4:
		c = CHAR_4;
		break;
	case 5:
		c = CHAR_5;
		break;
	case 6:
		c = CHAR_6;
		break;
	case 7:
		c = CHAR_7;
		break;
	case 8:
		c = CHAR_8;
		break;
	case 9:
		c = CHAR_9;
		break;
	}

	return c;
}

void display_t::print_g(int index, int x)
{
	print_g(index, to_custom_char(x));
}

void display_t::print_g(int index, custom_char c)
{	
	int offset = index*CHAR_WIDTH;
	switch (c)
	{
	case thermograph::CHAR_0:
		print_0(offset);
		break;
	case thermograph::CHAR_1:
		print_1(offset);
		break;
	case thermograph::CHAR_2:
		print_2(offset);
		break;
	case thermograph::CHAR_3:
		print_3(offset);
		break;
	case thermograph::CHAR_4:
		print_4(offset);
		break;
	case thermograph::CHAR_5:
		print_5(offset);
		break;
	case thermograph::CHAR_6:
		print_6(offset);
		break;
	case thermograph::CHAR_7:
		print_7(offset);
		break;
	case thermograph::CHAR_8:
		print_8(offset);
		break;
	case thermograph::CHAR_9:
		print_9(offset);
		break;
	case thermograph::CHAR_DEG:
		print_deg(offset);
		break;
	case thermograph::CHAR_C:
		print_c(offset);
		break;
	case thermograph::CHAR_QUESTION:
		print_question(offset);
		break;
	}
}

void display_t::print_g(const custom_char c[4])
{
	for(size_t i = 0; i < 4; i++)
	{
		print_g(i, c[i]);
	}

	_bitmap.update();
}

void display_t::print_0(int offset)
{
	/********************/
	/*** +----XX----+ ***/
	/*** |--XX--XX--| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** +XX------XX+ ***/
	/********************/
	/*** +XX------XX+ ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |--XX--XX--| ***/
	/*** +----XX----+ ***/
	/********************/
	_bitmap.line(offset + 0,  2, offset + 2,  0, ON);
	_bitmap.line(offset + 2,  0, offset + 4,  2, ON);
	_bitmap.line(offset + 4,  2, offset + 4, 13, ON);
	_bitmap.line(offset + 4, 13, offset + 2, 15, ON);
	_bitmap.line(offset + 2, 15, offset + 0, 13, ON);
	_bitmap.line(offset + 0, 13, offset + 0,  2, ON);
}

void display_t::print_1(int offset)
{
	/********************/
	/*** +--------XX+ ***/
	/*** |------XXXX| ***/
	/*** |----XX--XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** +--------XX+ ***/
	/********************/
	/*** +--------XX+ ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** +--------XX+ ***/
	/********************/
	_bitmap.line(offset + 4, 0, offset + 4, 15, ON);
	_bitmap.line(offset + 2, 2, offset + 4,  0, ON);
}

void display_t::print_2(int offset)
{
	/********************/
	/*** +----XX----+ ***/
	/*** |--XX--XX--| ***/
	/*** |XX------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |------XX--| ***/
	/*** |------XX--| ***/
	/*** +----XX----+ ***/
	/********************/
	/*** +----XX----+ ***/
	/*** |--XX------| ***/
	/*** |--XX------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** +XXXXXXXXXX+ ***/
	/********************/
	_bitmap.line(offset + 0,  3, offset + 0,  2, ON);
	_bitmap.line(offset + 0,  2, offset + 2,  0, ON);
	_bitmap.line(offset + 2,  0, offset + 4,  2, ON);
	_bitmap.line(offset + 4,  2, offset + 4,  4, ON);

	_bitmap.line(offset + 3,  5, offset + 3,  6, ON);
	_bitmap.line(offset + 2,  7, offset + 2,  8, ON);
	_bitmap.line(offset + 1,  9, offset + 1, 10, ON);

	_bitmap.line(offset + 0, 11, offset + 0, 15, ON);
	_bitmap.line(offset + 0, 15, offset + 4, 15, ON);
}

void display_t::print_3(int offset)
{
	/********************/
	/*** +----------+ ***/
	/*** |XXXXXXXXXX| ***/
	/*** |--------XX| ***/
	/*** |------XX--| ***/
	/*** |----XX----| ***/
	/*** |--XX------| ***/
	/*** |XXXXXX----| ***/
	/*** +------XX--+ ***/
	/********************/
	/*** +--------XX+ ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |XX----XX--| ***/
	/*** +--XXXX----+ ***/
	/********************/
	_bitmap.line(offset + 0,  0, offset + 4,  0, ON);
	_bitmap.line(offset + 4,  0, offset + 0,  5, ON);
	_bitmap.line(offset + 0,  5, offset + 2,  5, ON);
	_bitmap.line(offset + 2,  5, offset + 4,  7, ON);
	_bitmap.line(offset + 4,  7, offset + 4, 13, ON);
	_bitmap.line(offset + 4, 13, offset + 2, 15, ON);
	_bitmap.line(offset + 2, 15, offset + 1, 15, ON);
	_bitmap.line(offset + 1, 15, offset + 0, 14, ON);
}

void display_t::print_4(int offset)
{
	/********************/
	/*** +XX------XX+ ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** +XX------XX+ ***/
	/********************/
	/*** +XX------XX+ ***/
	/*** |XXXXXXXXXX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** +--------XX+ ***/
	/********************/
	_bitmap.line(offset + 0, 0, offset + 0, 9, ON);
	_bitmap.line(offset + 4, 0, offset + 4, 15, ON);
	_bitmap.line(offset + 0, 9, offset + 4, 9, ON);
}

void display_t::print_5(int offset)
{
	/********************/
	/*** +XXXXXXXXXX+ ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XXXXXX----| ***/
	/*** +------XX--+ ***/
	/********************/
	/*** +--------XX+ ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |XX----XX--| ***/
	/*** +--XXXX----+ ***/
	/********************/
	_bitmap.line(offset + 0,  0, offset + 4,  0, ON);
	_bitmap.line(offset + 0,  0, offset + 0,  6, ON);
	_bitmap.line(offset + 0,  6, offset + 2,  6, ON);
	_bitmap.line(offset + 2,  6, offset + 4,  8, ON);
	_bitmap.line(offset + 4,  8, offset + 4, 13, ON);
	_bitmap.line(offset + 4, 13, offset + 2, 15, ON);
	_bitmap.line(offset + 2, 15, offset + 1, 15, ON);
	_bitmap.line(offset + 1, 15, offset + 0, 14, ON);
}

void display_t::print_6(int offset)
{
	/********************/
	/*** +----XX----+ ***/
	/*** |--XX--XX--| ***/
	/*** |XX------XX| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--XX----| ***/
	/*** |XXXX--XX--| ***/
	/*** +XX------XX+ ***/
	/********************/
	/*** +XX------XX+ ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |--XX--XX--| ***/
	/*** +----XX----+ ***/
	/********************/
	_bitmap.line(offset + 4,  2, offset + 2,  0, ON);
	_bitmap.line(offset + 2,  0, offset + 0,  2, ON);
	_bitmap.line(offset + 0,  2, offset + 0, 13, ON);
	_bitmap.line(offset + 0, 13, offset + 2, 15, ON);
	_bitmap.line(offset + 2, 15, offset + 4, 13, ON);
	_bitmap.line(offset + 4, 13, offset + 4,  7, ON);
	_bitmap.line(offset + 4,  7, offset + 2,  5, ON);
	_bitmap.line(offset + 2,  5, offset + 0,  7, ON);
}

void display_t::print_7(int offset)
{
	/********************/
	/*** +XXXXXXXXXX+ ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |------XX--| ***/
	/*** |------XX--| ***/
	/*** |------XX--| ***/
	/*** +----XX----+ ***/
	/********************/
	/*** +----XX----+ ***/
	/*** |----XX----| ***/
	/*** |--XX------| ***/
	/*** |--XX------| ***/
	/*** |--XX------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** +XX--------+ ***/
	/********************/
	_bitmap.line(offset + 0,  0, offset + 4,  0, ON);
	_bitmap.line(offset + 4,  0, offset + 4,  3, ON);
	_bitmap.line(offset + 3,  4, offset + 3,  6, ON);
	_bitmap.line(offset + 2,  7, offset + 2,  9, ON);
	_bitmap.line(offset + 1, 10, offset + 1, 12, ON);
	_bitmap.line(offset + 0, 13, offset + 0, 15, ON);
}

void display_t::print_8(int offset)
{
	/********************/
	/*** +----XX----+ ***/
	/*** |--XX--XX--| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |--XX--XX--| ***/
	/*** +----XX----+ ***/
	/********************/
	/*** +--XX--XX--+ ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |--XX--XX--| ***/
	/*** +----XX----+ ***/
	/********************/
	_bitmap.line(offset + 0,  2, offset + 2,  0, ON);
	_bitmap.line(offset + 2,  0, offset + 4,  2, ON);
	_bitmap.line(offset + 4,  2, offset + 4,  5, ON);
	_bitmap.line(offset + 4,  5, offset + 2,  7, ON);
	_bitmap.line(offset + 2,  7, offset + 0,  5, ON);
	_bitmap.line(offset + 0,  5, offset + 0,  2, ON);

	_bitmap.line(offset + 0, 13, offset + 2, 15, ON);
	_bitmap.line(offset + 2, 15, offset + 4, 13, ON);
	_bitmap.line(offset + 4, 13, offset + 4,  9, ON);
	_bitmap.line(offset + 4,  9, offset + 2,  7, ON);
	_bitmap.line(offset + 2,  7, offset + 0,  9, ON);
	_bitmap.line(offset + 0,  9, offset + 0,  13, ON);
}

void display_t::print_9(int offset)
{
	/********************/
	/*** +----XX----+ ***/
	/*** |--XX--XX--| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** |XX------XX| ***/
	/*** +--XX--XXXX+ ***/
	/********************/
	/*** +----XX--XX+ ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |XX------XX| ***/
	/*** |--XX--XX--| ***/
	/*** +----XX----+ ***/
	/********************/
	_bitmap.line(offset + 0,  2, offset + 2,  0, ON);
	_bitmap.line(offset + 2,  0, offset + 4,  2, ON);
	_bitmap.line(offset + 4,  2, offset + 4, 13, ON);
	_bitmap.line(offset + 4, 13, offset + 2, 15, ON);
	_bitmap.line(offset + 2, 15, offset + 0, 13, ON);
	_bitmap.line(offset + 0,  2, offset + 0,  6, ON);
	_bitmap.line(offset + 0,  6, offset + 2,  8, ON);
	_bitmap.line(offset + 2,  8, offset + 4,  6, ON);
}

void display_t::print_deg(int offset)
{
	/********************/
	/*** +--XXXXXX--+ ***/
	/*** |--XX--XX--| ***/
	/*** |--XXXXXX--| ***/
	/*** |----------| ***/
	/*** |----------| ***/
	/*** |----------| ***/
	/*** |----------| ***/
	/*** +----------+ ***/
	/********************/
	/*** +----------+ ***/
	/*** |----------| ***/
	/*** |----------| ***/
	/*** |----------| ***/
	/*** |----------| ***/
	/*** |----------| ***/
	/*** |----------| ***/
	/*** +----------+ ***/
	/********************/
	_bitmap.line(offset + 1, 0, offset + 3, 0, ON);
	_bitmap.line(offset + 1, 0, offset + 1, 3, ON);
	_bitmap.line(offset + 1, 3, offset + 3, 3, ON);
	_bitmap.line(offset + 3, 0, offset + 3, 3, ON);
}

void display_t::print_c(int offset)
{
	/********************/
	/*** +----XXXXXX+ ***/
	/*** |--XX------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** +XX--------+ ***/
	/********************/
	/*** +XX--------+ ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |XX--------| ***/
	/*** |--XX------| ***/
	/*** +----XXXXXX+ ***/
	/********************/
	_bitmap.line(offset + 0,  2, offset + 2,  0, ON);
	_bitmap.line(offset + 2,  0, offset + 4,  2, ON);
	_bitmap.line(offset + 0,  2, offset + 0, 13, ON);
	_bitmap.line(offset + 0, 13, offset + 2, 15, ON);
	_bitmap.line(offset + 2, 15, offset + 4, 13, ON);
}

void display_t::print_question(int offset)
{
	/********************/
	/*** +----XX----+ ***/
	/*** |--XX--XX--| ***/
	/*** |XX------XX| ***/
	/*** |--------XX| ***/
	/*** |--------XX| ***/
	/*** |------XX--| ***/
	/*** |------XX--| ***/
	/*** +----XX----+ ***/
	/********************/
	/*** +----XX----+ ***/
	/*** |----XX----| ***/
	/*** |----XX----| ***/
	/*** |----XX----| ***/
	/*** |----XX----| ***/
	/*** |----XX----| ***/
	/*** |----------| ***/
	/*** +----XX----+ ***/
	/********************/
	_bitmap.line(offset + 0,  3, offset + 0,  2, ON);
	_bitmap.line(offset + 0,  2, offset + 2,  0, ON);
	_bitmap.line(offset + 2,  0, offset + 4,  2, ON);
	_bitmap.line(offset + 4,  2, offset + 4,  4, ON);
	_bitmap.line(offset + 3,  5, offset + 3, 13, ON);
	_bitmap.line(offset + 3, 15, offset + 3, 15, ON);
}