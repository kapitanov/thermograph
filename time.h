#pragma once

#include <inttypes.h>

namespace thermograph
{
	struct time_t
	{
		uint8_t h;
		uint8_t min;
		uint8_t sec;
	};

	struct sys_time_t
	{
		uint16_t h;
		uint8_t  min;
		uint8_t  sec;
		uint16_t ms;
	};

	class time_service_t
	{
	public:
		const time_t get_time() const;
		const sys_time_t get_systime() const;
	};

	extern time_service_t time_service;
}