#include "Arduino.h"
#include "button.h"
#include "data_history.h"
#include "display.h"
#include "log.h"
#include "mode.h"
#include "sensor.h"
#include "util.h"

using namespace thermograph;

/*
************************************************************************
*	condensed_display_mode_t
*	Condensed display app mode
************************************************************************
*/

/**
*	Informs an app mode about activation
**/
void condensed_display_mode_t::enter()
{
	_last_temperature = 0;
	_last_humidity = 0;
	display.text().clear();
	print();
}

/**
*	Handles empty button event
**/
mode_event condensed_display_mode_t::handle()
{
	print();
	return ME_NONE;
}

/**
*	Handles button event
*	@param		btn	a button code
*	@returns	an event code
**/
mode_event condensed_display_mode_t::handle(const button btn)
{
	switch (btn)
	{
	case BTN_UP:
	case BTN_DOWN:
		_sensor_id = (_sensor_id == SENSOR_ID_INDOOR) ? SENSOR_ID_OUTDOOR : SENSOR_ID_INDOOR;
	default:
		break;
	}

	_last_temperature = 0;
	_last_humidity = 0;
	print();
	return ME_NONE;
}

/**
*	Prints current values
**/
void condensed_display_mode_t::print()
{
	bool temperature_outdated = false;
	optional_t<temperature_t> temperature = sensor.get_temperature(_sensor_id);
	if(temperature.has_value() && _last_temperature != temperature.value())
	{
		_last_temperature = static_cast<int>(10 * temperature.value());
		temperature_outdated = true;
	}

	bool humidity_outdated = false;
	optional_t<humidity_t> humidity = sensor.get_humidity(_sensor_id);
	if(humidity.has_value() && _last_humidity != humidity.value())
	{
		_last_humidity = static_cast<int>(10 * humidity.value());
		humidity_outdated = true;
	}

	if(!temperature_outdated && !humidity_outdated)
	{
		return;
	}

	display.text().clear();

	display.text().setCursor(0, 0);
	display.text().print("Temp  ");
	if(_last_temperature >= 100)
	{
		display.text().print(util::get_number(_last_temperature, 2));
	}
	display.text().print(util::get_number(_last_temperature, 1));
	display.text().print('.');
	display.text().print(util::get_number(_last_temperature, 0));
	display.text().print('\xDF');
	display.text().print('C');
	display.text().print(' ');

	switch (_sensor_id)
	{
	case SENSOR_ID_OUTDOOR:
		display.text().print("out");
		break;
	case SENSOR_ID_INDOOR:
		display.text().print("in");
		break;
	default:
		break;
	}

	display.text().setCursor(0, 1);
	display.text().print("Humidity  ");
	if(_last_humidity >= 100)
	{
		display.text().print(util::get_number(_last_humidity, 2));
	}
	display.text().print(util::get_number(_last_humidity, 1));
	display.text().print('.');
	display.text().print(util::get_number(_last_humidity, 0));
	display.text().print('%');

	log.debug(
		F("temperature_and_time_display_mode\tprint_temperature(): temperature = %d deg C, humidity = %d%%"), 
		_last_temperature / 10,
		_last_humidity / 10);
}