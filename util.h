#include "Arduino.h"

namespace thermograph
{
	namespace util
	{
		/**
		 *	Gets a decimal value from a number
		 *	@param		value	an input value
		 *	@param		index	decimal place index
		 *	@returns	a decimal value from a number's decimal place
		 **/
		int get_number(int value, int index);

		/**
		 *	Prints a formatted decimal number
		 *	@param	print		printer
		 *	@param	precision	decimal places count
		 *	@param	placeholder	decimal placeholder
		 **/
		void print_number(Print& print, int value, int precision, char placeholder = ' ');
	}
}