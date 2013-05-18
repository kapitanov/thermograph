#include "util.h"

/**
 *	Computes a power of 10
 *	@param		pow	power
 *	@returns	a value of 10^pow
 **/
int powerOf10(int pow)
{
	int16_t x = 1;
	for (int i = 0; i < pow; i++)
	{
		x *= 10;
	}

	return x;
}

/**
 *	Gets a decimal value from a number
 *	@param		value	an input value
 *	@param		index	decimal place index
 *	@returns	a decimal value from a number's decimal place
 **/
int thermograph::util::get_number(int value, int index)
{
	int16_t d1 = powerOf10(index + 1);
	int16_t d2 = powerOf10(index);
	int16_t x = (value % d1 - value % d2) / d2;
	return x;
}

/**
 *	Prints a formatted decimal number
 *	@param	print		printer
 *	@param	precision	decimal places count
 *	@param	placeholder	decimal placeholder
 **/
void thermograph::util::print_number(Print& print, int value, int precision, char placeholder)
{
	for (int i = precision - 1; i >= 0; i--)
	{
		int16_t x = get_number(value, i);

		if(x == 0 && i != 0)
		{
			print.print(placeholder);
		}
		else
		{
			print.print(x);
		}
	}
}