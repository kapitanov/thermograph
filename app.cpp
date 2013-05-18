#include "Arduino.h"
#include "_config.h"
#include "app.h"
#include "display.h"
#include "log.h"
#include "sensor.h"

using namespace thermograph;

/*
 ************************************************************************
 *	app_t
 *	Root app's class
 ************************************************************************
 */

/**
 *	Constructor
 **/
app_t::app_t()
{ }

/**
 *	Initializes an app
 **/
void app_t::init()
{
	// Initialize logging
	log.init(9600);
	log.info(F("app\tboot"));

	// Initialize I/O
	display.init();
	sensor.init();
	
	// Assign app mode pointers
	_modes[0] = &_temperature_display_mode;
	_modes[1] = &_temperature_and_time_display_mode;
	_modes[2] = &_temperature_chart_display_mode;

	// Initialize app modes
	for (int i = 0; i < sizeof(_modes)/sizeof(mode_t*); i++)
	{
		_modes[i]->init();
	}
	
	log.info(F("app\tstarted"));
}

/**
 *	Runs an app
 **/
void app_t::run()
{
	log.debug(F("app\trun"));

	// Update sensor values conditionless
	sensor.update(sensor_service_t::UP_FORCE);

	// Initialize active app mode
	_modes[_mode_index]->enter();

	while(true)
	{
		// Update sensor if nessesary
		sensor.update();

		// Handle user I/O event and update display
		mode_event e = _modes[_mode_index]->handle_events();

		switch (e)
		{
		case thermograph::ME_NONE:
			break;

		case thermograph::ME_SWITCH_MODE:

			// Cycle through app modes
			_mode_index++;
			if(_mode_index >= sizeof(_modes)/sizeof(mode_t*))
			{
				_mode_index = 0;
			}

			log.info(F("app\trun mode switced to #%d"), _mode_index);

			// Initialize active app mode
			_modes[_mode_index]->enter();
			break;
		}
	}
}