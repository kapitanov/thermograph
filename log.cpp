#include "Arduino.h"
#include <avr/pgmspace.h>
#include "log.h"
#include "util.h"

using namespace thermograph;

/**
*	Logger static instance
**/
log_t thermograph::log;

/*
 ************************************************************************
 *	log_event_t
 *	Log event writer
 ************************************************************************
 */

/**
*	Constructor
*	@param	enable	indicates whether current log writer instance is allowed to write any data into log
**/
log_event_t::log_event_t(bool enable)
	: _enable(enable)
{ }

/**
*	Destructor
**/
log_event_t::~log_event_t()
{
	if(_enable)
	{
		Serial.println();
		Serial.flush();
	}
}

/**
*	Appends a formatted text into log
*	@param	msg	format string
**/
void log_event_t::printf(const char* msg, ...)
{
	if(_enable)
	{
		va_list args;
		va_start(args, msg);
		log.print_message(msg, args);
	}
}

/**
*	Appends a formatted text into log
*	@param	msg	format string
**/
void log_event_t::printf(__FlashStringHelper* msg, ...)
{
	if(_enable)
	{
		va_list args;
		va_start(args, msg);
		log.print_message(msg, args);
	}
}

/*
 ************************************************************************
 *	log_t
 *	Logger class
 ************************************************************************
 */

/** public members **/

/**
*	Initializes logging
*	@param	baud	serial port baud rate
**/
void log_t::init(const long baud)
{
	Serial.begin(baud);
}

/**
*	Writes a formatted message with ERR log level into log
*	@param	msg	format string
**/
void log_t::error(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_ERROR, msg, args);
}

/**
*	Writes a formatted message with INF log level into log
*	@param	msg	format string
**/
void log_t::info(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_INFO, msg, args);
}

/**
*	Writes a formatted message with DBG log level into log
*	@param	msg	format string
**/
void log_t::debug(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_DEBUG, msg, args);
}

/**
*	Writes a formatted message with ERR log level into log
*	@param	msg	format string
**/
void log_t::error(__FlashStringHelper* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_ERROR, msg, args);
}

/**
*	Writes a formatted message with INF log level into log
*	@param	msg	format string
**/
void log_t::info(__FlashStringHelper* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_INFO, msg, args);
}

/**
*	Writes a formatted message with DBG log level into log
*	@param	msg	format string
**/
void log_t::debug(__FlashStringHelper* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_DEBUG, msg ,args);
}

/**
*	Starts writing an event using log event writer.
*	@param		level	log level
*	@returns	log event writer
**/
log_event_t log_t::begin_event(log_level level)
{
#ifndef ENABLE_DEBUG_LOGGING
	if(level == LOG_DEBUG)
	{
		return log_event_t(false);
	}
#endif

	print_header(level);
	return log_event_t(true);
}

/**	private members	**/

/**
*	Writes a formatted message into log
*	@param	level	log level
*	@param	format	format string
*	@params	args	format arguments list
**/
void log_t::print(log_level level, const char* format, va_list args)
{
#ifndef ENABLE_DEBUG_LOGGING
	if(level == LOG_DEBUG)
	{
		return;
	}
#endif

	print_header(level);

	print_message(format, args);
	Serial.println();
	Serial.flush();
}

/**
*	Writes a formatted message into log
*	@param	level	log level
*	@param	format	format string
*	@params	args	format arguments list
**/
void log_t::print(log_level level, __FlashStringHelper* format, va_list args)
{
#ifndef ENABLE_DEBUG_LOGGING
	if(level == LOG_DEBUG)
	{
		return;
	}
#endif

	print_header(level);

	print_message(format, args);
	Serial.println();
	Serial.flush();
}

/**
*	Writes a log message header
*	@param	level	log level
*/
void log_t::print_header(log_level level)
{
	__FlashStringHelper* header;
	switch (level)
	{
	case LOG_ERROR:
		header = F("ERR");
		break;
	case LOG_INFO:
		header = F("INF");
		break;
	case LOG_DEBUG:
		header = F("DBG");
		break;
	default:
		break;
	}

	// TIME LOCAL
	print_time(time_service.get_systime());
	Serial.print('\t');
	// TIME GLOBAL
	print_time(time_service.get_time());
	Serial.print('\t');

	// LOG LEVEL
	Serial.print(header);
	Serial.print('\t');
}

/**
*	Writes a formatted global time into log
*	@param	time global time
**/
void log_t::print_time(const time_t& time)
{
	print_formatted_number(time.h, 2);
	Serial.print(':');
	print_formatted_number(time.min, 2);
	Serial.print(':');
	print_formatted_number(time.sec, 2);
}

/**
*	Writes a formatted local time into log
*	@param	time local time
**/
void log_t::print_time(const sys_time_t& time)
{
	print_formatted_number(time.h, 4, ' ');
	Serial.print(':');
	print_formatted_number(time.min, 2);
	Serial.print(':');
	print_formatted_number(time.sec, 2);
	Serial.print('.');
	print_formatted_number(time.ms, 3);
}

/**
*	Writes a formatted decimal number into log
*	@param	value		a number to write
*	@param	precision	amount of decimal places
*	@param	placeholder	a placeholder character
**/
void log_t::print_formatted_number(int value, int precision, char placeholder)
{
	util::print_number(Serial, value, precision, placeholder);
	return;
}

/**
*	Writes a formatted message into log
*	@param	format	format string
*	@params	args	format arguments list
**/
void log_t::print_message(__FlashStringHelper* format_ptr, va_list args) 
{
	uint16_t address = reinterpret_cast<uint16_t>(format_ptr);
	char c;
	size_t i = -1;
	while(true)
	{
		i++;
		c = pgm_read_byte(address + i);
		if(c == '\0')
		{
			break;
		}

		if (c == '%') 
		{
			++i;
			c = pgm_read_byte(address + i);

			if (c == '\0') break;
			if (c == '%') 
			{
				Serial.print(c);
				continue;
			}

			if( c == 's' ) 
			{
				register char *s = (char *)va_arg( args, int );
				Serial.print(s);
				continue;
			}

			if( c == 'd' || c == 'i')
			{
				Serial.print(va_arg( args, int ),DEC);
				continue;
			}

			if( c == 'x' )
			{
				Serial.print(va_arg( args, int ),HEX);
				continue;
			}

			if( c == 'X' )
			{
				Serial.print("0x");
				Serial.print(va_arg( args, int ),HEX);
				continue;
			}

			if( c == 'b' ) 
			{
				Serial.print(va_arg( args, int ),BIN);
				continue;
			}

			if( c == 'B' ) 
			{
				Serial.print("0b");
				Serial.print(va_arg( args, int ),BIN);
				continue;
			}

			if( c == 'l' )
			{
				Serial.print(va_arg( args, long ),DEC);
				continue;
			}


			if( c == 'c' )
			{
				Serial.print(va_arg( args, int ));
				continue;
			}

			if( c == 't' )
			{
				if (va_arg( args, int ) == 1)
				{
					Serial.print("T");
				}
				else 
				{
					Serial.print("F");
				}
				continue;
			}

			if( c == 'T' )
			{
				if (va_arg( args, int ) == 1) 
				{
					Serial.print("true");
				}
				else 
				{
					Serial.print("false");
				}
				continue;
			}

			if( c == 'u' ) 
			{
				time_t* t = (time_t *)va_arg( args, int );
				print_time(*t);
				continue;
			}
			if( c == 'U' ) 
			{
				sys_time_t* t = (sys_time_t *)va_arg( args, int );
				print_time(*t);
				continue;
			}

			if( c == 'f' ) 
			{
				float x = *(float *)va_arg( args, int );
				Serial.print(x);
				continue;
			}
			if( c == 'F' ) 
			{
				double x = *(double *)va_arg( args, int );
				Serial.print(x);
				continue;
			}
		}

		Serial.print(c);
	}
}

/**
*	Writes a formatted message into log
*	@param	format	format string
*	@params	args	format arguments list
**/
void log_t::print_message(const char *format, va_list args) 
{
	// loop through format string
	for (; *format != 0; ++format)
	{
		if (*format == '%') 
		{
			++format;
			if (*format == '\0') break;
			if (*format == '%') 
			{
				Serial.print(*format);
				continue;
			}

			if( *format == 's' ) 
			{
				register char *s = (char *)va_arg( args, int );
				Serial.print(s);
				continue;
			}

			if( *format == 'd' || *format == 'i')
			{
				Serial.print(va_arg( args, int ),DEC);
				continue;
			}

			if( *format == 'x' )
			{
				Serial.print(va_arg( args, int ),HEX);
				continue;
			}

			if( *format == 'X' )
			{
				Serial.print("0x");
				Serial.print(va_arg( args, int ),HEX);
				continue;
			}

			if( *format == 'b' ) 
			{
				Serial.print(va_arg( args, int ),BIN);
				continue;
			}

			if( *format == 'B' ) 
			{
				Serial.print("0b");
				Serial.print(va_arg( args, int ),BIN);
				continue;
			}

			if( *format == 'l' )
			{
				Serial.print(va_arg( args, long ),DEC);
				continue;
			}


			if( *format == 'c' )
			{
				Serial.print(va_arg( args, int ));
				continue;
			}

			if( *format == 't' )
			{
				if (va_arg( args, int ) == 1)
				{
					Serial.print("T");
				}
				else 
				{
					Serial.print("F");
				}
				continue;
			}

			if( *format == 'T' )
			{
				if (va_arg( args, int ) == 1) 
				{
					Serial.print("true");
				}
				else 
				{
					Serial.print("false");
				}
				continue;
			}

			if( *format == 'u' ) 
			{
				time_t* t = (time_t *)va_arg( args, int );
				print_time(*t);
				continue;
			}
			if( *format == 'U' ) 
			{
				sys_time_t* t = (sys_time_t *)va_arg( args, int );
				print_time(*t);
				continue;
			}

			if( *format == 'f' ) 
			{
				float x = *(float *)va_arg( args, int );
				Serial.print(x);
				continue;
			}
			if( *format == 'F' ) 
			{
				double x = *(double *)va_arg( args, int );
				Serial.print(x);
				continue;
			}
		}

		Serial.print(*format);
	}
}
