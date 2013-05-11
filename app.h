#pragma once

#include "mode.h"

namespace thermograph
{
	class app_t
	{
	public:
		app_t();

		void init();
		void run();

	private:
		temperature_display_mode_t _temperature_display_mode;
		temperature_and_time_display_mode_t _temperature_and_time_display_mode;
		temperature_chart_display_mode_t _temperature_chart_display_mode;

		mode_t* _modes[3];
		uint8_t _mode_index;
	};
}