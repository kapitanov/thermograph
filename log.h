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
	/**
	 *	Log text format string
	 *	+-----------+-------------------------------------+-------------------+
	 *	|	Format	|	Description							|	Argument type	|
	 *	+-----------+-------------------------------------+-------------------+
	 *	|	%%		|	'%'									|	-				|
	 *	|	%s		|	string								|	char*			|
	 *	|	%c		|	char								|	char			|
	 *	|	%d		|	integer								|	int				|
	 *	|	%l		|	long								|	long			|
	 *	|	%x		|	integer (hex)						|	int				|
	 *	|	%X		|	integer (hex with '0x' prefix)		|	int				|
	 *	|	%b		|	integer (binary)					|	int				|
	 *	|	%B		|	integer (binary with '0b' prefix)	|	int				|
	 *	|	%t		|	boolean ('t' or 'f')				|	bool			|
	 *	|	%T		|	boolear ('true' or 'false')			|	bool			|
	 *	|	%u		|	global time							|	time_t*			|
	 *	|	%U		|	system time							|	sys_time_t*		|
	 *	|	%f		|	float point value					|	float*			|
	 *	|	%F		|	float point value					|	double*			|
	 *	+-----------+-------------------------------------+-------------------+
	 **/

	/**
	 *	Log levels enumeration
	 **/
	enum log_level
	{
		/**
		 *	ERR log level
		 **/
		LOG_ERROR,

		/**
		 *	INF log level
		 **/
		LOG_INFO,

		/**
		 *	DBG log level
		 **/
		LOG_DEBUG
	};
	
	/**
	 *	Log event writer
	 **/
	class log_event_t
	{
	public:
		/**
		 *	Constructor
		 *	@param	enable	indicates whether current log writer instance is allowed to write any data into log
		 **/
		log_event_t(bool enable);

		/**
		 *	Destructor
		 **/
		~log_event_t();

		/**
		 *	Appends a formatted text into log
		 *	@param	msg	format string
		 **/
		void printf(const char* msg, ...);

		/**
		 *	Appends a formatted text into log
		 *	@param	msg	format string
		 **/
		void printf(__FlashStringHelper* msg, ...);

	private:
		/**
		 *	Indicates whether current log writer instance is allowed to write any data into log
		 **/
		bool _enable;
	};

	/**
	 *	Logger class
	 **/
	class log_t
	{
	public:
		/**
		 *	Initializes logging
		 *	@param	baud	serial port baud rate
		 **/
		void init(const long baud);

		/**
		 *	Writes a formatted message with ERR log level into log
		 *	@param	msg	format string
		 **/
		void error(const char* msg, ...);

		/**
		 *	Writes a formatted message with INF log level into log
		 *	@param	msg	format string
		 **/
		void info(const char* msg, ...);

		/**
		 *	Writes a formatted message with DBG log level into log
		 *	@param	msg	format string
		 **/
		void debug(const char* msg, ...);


		/**
		 *	Writes a formatted message with ERR log level into log
		 *	@param	msg	format string
		 **/
		void error(__FlashStringHelper* msg, ...);

		/**
		 *	Writes a formatted message with INF log level into log
		 *	@param	msg	format string
		 **/
		void info(__FlashStringHelper* msg, ...);

		/**
		 *	Writes a formatted message with DBG log level into log
		 *	@param	msg	format string
		 **/
		void debug(__FlashStringHelper* msg, ...);


		/**
		 *	Starts writing an event using log event writer.
		 *	@param		level	log level
		 *	@returns	log event writer
		 **/
		log_event_t begin_event(log_level level);

	private:
		friend class log_event_t;

		/**
		 *	Writes a formatted message into log
		 *	@param	level	log level
		 *	@param	format	format string
		 *	@params	args	format arguments list
		 **/
		void print(log_level level, const char* format, va_list args);

		/**
		 *	Writes a formatted message into log
		 *	@param	level	log level
		 *	@param	format	format string
		 *	@params	args	format arguments list
		 **/
		void print(log_level level, __FlashStringHelper* format, va_list args);

		/**
		 *	Writes a log message header
		 *	@param	level	log level
		 **/
		void print_header(log_level level);

		/**
		 *	Writes a formatted global time into log
		 *	@param	time global time
		 **/
		void print_time(const time_t& time);

		/**
		 *	Writes a formatted local time into log
		 *	@param	time local time
		 **/
		void print_time(const sys_time_t& time);

		/**
		 *	Writes a formatted decimal number into log
		 *	@param	value		a number to write
		 *	@param	precision	amount of decimal places
		 *	@param	placeholder	a placeholder character
		 **/
		void print_formatted_number(int value, int precision, char placeholder = '0');

		/**
		 *	Writes a formatted message into log
		 *	@param	format	format string
		 *	@params	args	format arguments list
		 **/
		void print_message(const char* format, va_list args);

		/**
		 *	Writes a formatted message into log
		 *	@param	format	format string
		 *	@params	args	format arguments list
		 **/
		void print_message(__FlashStringHelper* format, va_list args);
	};

	/**
	 *	Logger static instance
	 **/
	extern log_t log;
}




