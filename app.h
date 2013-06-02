#pragma once

#include "mode.h"

namespace thermograph
{
	/**
	 *	Root app's class
	 **/
	class app_t
	{
	public:
		/**
		*	Constructor
		**/
		app_t();

		/**
		 *	Initializes an app
		 **/
		void init();

		/**
		 *	Runs an app
		 **/
		void run();

	private:
		/**
		 *	App mode - temperature display
		 **/
		expanded_display_mode_t _expanded_display_mode;

		/**
		 *	App mode - condensed display
		 **/
		condensed_display_mode_t _condensed_display_mode;

		/**
		 *	App mode - temperature chart display
		 **/
		temperature_chart_display_mode_t _temperature_chart_display_mode;

		/**
		 *	An array of all available app modes
		 **/
		mode_t* _modes[3];

		/**
		 *	An index of active app mode in "_modes" array
		 */
		uint8_t _mode_index;
	};
}