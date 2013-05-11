#pragma once

#include "_config.h"
#include <stdarg.h>
#include "Arduino.h"
extern "C" 
{
#include <avr/io.h>
}

#include "time.h"

namespace thermograph
{
	/*
	 * Log text format string
	 * %%		'%'									none
	 * %s		string								char*
	 * %c		char								char
	 * %d		integer								int
	 * %l		long								long
	 * %x		integer (hex)						int
	 * %X		integer (hex with '0x' prefix)		int
	 * %b		integer (binary)					int
	 * %B		integer (binary with '0b' prefix)	int
	 * %t		boolean ('t' or 'f')				bool
	 * %T		boolear ('true' or 'false')			bool
	 * %u		global time							time_t*
	 * %U		system time							sys_time_t*
	 * %f		float point value					float*
	 * %F		float point value					double*
	 */

	enum log_level
	{
		LOG_ERROR,
		LOG_INFO,
		LOG_DEBUG
	};
	
	class log_event_t
	{
	public:
		log_event_t(bool enable);
		~log_event_t();
		void printf(const char* msg, ...);
		void printf(__FlashStringHelper* msg, ...);
	private:
		bool _enable;
	};

	class log_t
	{
	public:
		void init(const long baud);

		void error(const char* msg, ...);
		void info(const char* msg, ...);
		void debug(const char* msg, ...);

		void error(__FlashStringHelper* msg, ...);
		void info(__FlashStringHelper* msg, ...);
		void debug(__FlashStringHelper* msg, ...);

		log_event_t begin_event(log_level level);

	private:
		friend class log_event_t;

		void print(log_level level, const char* format, va_list args);
		void print(log_level level, __FlashStringHelper* format, va_list args);
		void print_header(log_level level);
		void print_time(const time_t& time);
		void print_time(const sys_time_t& time);
		void print_formatted_number(int value, int precision, char placeholder = '0');
		void print_message(const char* format, va_list args);
		void print_message(__FlashStringHelper* format, va_list args);
	};

	extern log_t log;
}




