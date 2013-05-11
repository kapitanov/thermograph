#include "Arduino.h"

namespace thermograph
{
	namespace util
	{
		int get_number(int value, int index);
		void print_number(Print& print, int value, int precision, char placeholder = ' ');
	}
}