#pragma once

#include "LiquidCrystal.h"
#include "LCDBitmap.h"

namespace thermograph
{
	enum custom_char
	{
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
		CHAR_QUESTION
	};

	class display_t
	{
	public:
		display_t();

		void init();

		LiquidCrystal& text() { return _lcd; }
		LCDBitmap& graphics() { return _bitmap; }

		void print_g(int index, int c);
		void print_g(int index, custom_char c);
		void print_g(const custom_char c[4]);

		static custom_char to_custom_char(int x);

	private:
		LiquidCrystal _lcd;
		LCDBitmap _bitmap;

		static const int CHAR_WIDTH;

		void print_0(int offset);
		void print_1(int offset);
		void print_2(int offset);
		void print_3(int offset);
		void print_4(int offset);
		void print_5(int offset);
		void print_6(int offset);
		void print_7(int offset);
		void print_8(int offset);
		void print_9(int offset);
		void print_deg(int offset);
		void print_c(int offset);
		void print_question(int offset);
	};

	extern display_t display;
}