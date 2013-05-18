#pragma once

#include <inttypes.h>

namespace thermograph
{
	/**
	 * Global time type
	 **/
	struct time_t
	{
		/**
		 *	Hours
		 **/
		uint8_t h;

		/**
		 *	Minutes
		 **/
		uint8_t min;

		/**
		 *	Seconds
		 **/
		uint8_t sec;
	};

	/**
	 *	Local time type
	 **/
	struct sys_time_t
	{
		/**
		 *	Hours
		 **/
		uint16_t h;

		/**
		 *	Minutes
		 **/
		uint8_t  min;

		/**
		 *	Seconds
		 **/
		uint8_t  sec;

		/**
		 *	Milliseconds
		 **/
		uint16_t ms;
	};

	/**
	 *	Time service class
	 **/
	class time_service_t
	{
	public:
		/**
		 *	Gets current global time
		 *	@returns	current global time value
		 **/
		const time_t get_time() const;

		/**
		 *	Gets local global time
		 *	@returns	current local time value
		 **/
		const sys_time_t get_systime() const;
	};

	/**
	 *	Time service static instance
	 **/
	extern time_service_t time_service;
}