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
*	temperature_chart_display_mode_t
*	Temperature chart app mode
************************************************************************
*/

/**
*	Handles empty button event
**/
void temperature_chart_display_mode_t::enter()
{
	display.graphics().clear();
	display.graphics().clear_text();
	print_chart(true);
}

/**
*	Handles empty button event
**/
mode_event temperature_chart_display_mode_t::handle()
{
	print_chart(false);
	return ME_NONE;
}

/**
*	Handles button event
*	@param		btn	a button code
*	@returns	an event code
**/
mode_event temperature_chart_display_mode_t::handle(const button btn)
{
	switch (btn)
	{
	case BTN_LEFT:
	case BTN_RIGHT:
		_sensor_id = (_sensor_id == SENSOR_ID_INDOOR) ? SENSOR_ID_OUTDOOR : SENSOR_ID_INDOOR;
	default:
		break;
	}

	print_chart(false);
	return ME_NONE;
}

/**
*	Prints a temperature plot
*	@param	force	disable data revision check
**/
void temperature_chart_display_mode_t::print_chart(bool force)
{
	uint8_t rev = data_history.get_revision();
	if(rev == _last_rev && !force)
	{
		return;
	}

	byte points[data_history_t::DATA_POINTS_COUNT];
	data_history.get_points(_sensor_id, points);
	display.graphics().barGraph(20, points, ON, UPDATE);

	display.text().setCursor(0, 0);
	display.text().print("Temp chart");

	display.text().setCursor(0, 1);
	char* text;
	switch (_sensor_id)
	{
	case SENSOR_ID_OUTDOOR:
		text = "[outside]";
		break;
	case SENSOR_ID_INDOOR:
		text = "[room]";
		break;
	default:
		break;
	}
	display.text().print(text);

	_last_rev = rev;

	log.debug(F("temperature_chart_display_mode\tprint_chart(): rev = #%d"), rev);
}