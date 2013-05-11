#include "util.h"

int powerOf10(int pow)
{
	int16_t x = 1;
	for (int i = 0; i < pow; i++)
	{
		x *= 10;
	}

	return x;
}

int thermograph::util::get_number(int value, int index)
{
	int16_t d1 = powerOf10(index + 1);
	int16_t d2 = powerOf10(index);
	int16_t x = (value % d1 - value % d2) / d2;
	return x;
}

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