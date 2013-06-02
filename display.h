#pragma once

#include "LiquidCrystal.h"
#include "LCDBitmap.h"

namespace thermograph
{
	/**
	 *	Customized characters enumeration
	 **/
	enum custom_char
	{
		CHAR_NONE,
		CHAR_0,
		CHAR_1,
		CHAR_2,
		CHAR_3,
		CHAR_4,
		CHAR_5,
		CHAR_6,
		CHAR_7,
		CHAR_8,
		CHAR_9,
		CHAR_DEG,
		CHAR_C,
		CHAR_PERCENT,
		CHAR_QUESTION
	};

	/**
	 *	Display adapter class
	 **/
	class display_t
	{
	public:
		/**
		 *	Constructor
		 **/
		display_t();

		/**
		 *	Initializes the display
		 **/
		void init();

		/**
		 *	Gets the LCD adapter
		 *	@returns	A reference to an LCD adapter
		 **/
		LiquidCrystal& text() { return _lcd; }

		/**
		 *	Gets the LCD bitmap
		 *	@returns	A reference to an LCD bitmap
		 **/
		LCDBitmap& graphics() { return _bitmap; }

		/**
		 *	Prints a number on an LCD bitmap 
		 *	@param	index	an index of a character on the bitmap. Must be within [0, 3] range.
		 *	@param	x		a number to display. 
		 *					Must be within [0, 9] range, a '?' character will be displayed otherwise
		 **/
		void print_g(int index, int x);

		/**
		 *	Prints a characher on an LCD bitmap 
		 *	@param	index	an index of a character on the bitmap. Must be within [0, 3] range.
		 *	@param	c		a characher to display. 
		 **/
		void print_g(int index, custom_char c);

		/**
		 *	Prints 4 characters on an LCD bitmap
		 *	@param	c	an array of characters to print
		 **/
		void print_g(const custom_char c[4]);

		/**
		 *	Helper function to convert a number into a custom_char value
		 *	@param	x	a number to convert. 
		 *				Must be within [0, 9] range, a '?' character will be returned otherwise.
		 *	@returns	a value of custom_char
		 **/
		static custom_char to_custom_char(int x);

	private:
		/**
		 *	LCD adapter
		 **/
		LiquidCrystal _lcd;

		/**
		 *	LCD bitmap
		 **/
		LCDBitmap _bitmap;

		/**
		 *	LCD character width, in pixels
		 **/
		static const int CHAR_WIDTH;

		/**
		 *	Draws a '0' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_0(int offset);

		/**
		 *	Draws a '1' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_1(int offset);

		/**
		 *	Draws a '2' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_2(int offset);

		/**
		 *	Draws a '3' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_3(int offset);

		/**
		 *	Draws a '4' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_4(int offset);

		/**
		 *	Draws a '5' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_5(int offset);

		/**
		 *	Draws a '6' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_6(int offset);

		/**
		 *	Draws a '7' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_7(int offset);

		/**
		 *	Draws a '8' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_8(int offset);

		/**
		 *	Draws a '9' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_9(int offset);

		/**
		 *	Draws a degree sign character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_deg(int offset);

		/**
		 *	Draws a Celsium sign character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_c(int offset);

		/**
		 *	Draws a '%' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_percent(int offset);

		/**
		 *	Draws a '?' character onto LCD bitmap
		 *	@param	offset	an offset to draw a character, in pixels
		 **/
		void print_question(int offset);
	};

	/**
	 *	Display adapter static instance
	 **/
	extern display_t display;
}