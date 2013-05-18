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
 *	mode_t
 *	App mode base class
 ************************************************************************
 */

/**
 *	Handles events
 *	@returns	an event code
 **/
mode_event mode_t::handle_events()
{
	button btn = button_service.read_button();

	switch (btn)
	{
	case thermograph::BTN_NONE:
		return handle();

	case thermograph::BTN_SELECT:
		return ME_SWITCH_MODE;

	default:
		return handle(btn);
	}
}

/*
 ************************************************************************
 *	temperature_display_mode_t
 *	Temperature display app mode
 ************************************************************************
 */

/**
 *	Informs an app mode about activation
 **/
void temperature_display_mode_t::enter()
{
	_last_value = 0;
	display.text().clear();
	print_temperature();
}

/**
 *	Handles empty button event
 **/
mode_event temperature_display_mode_t::handle()
{
	print_temperature();
	return ME_NONE;
}

/**
 *	Handles button event
 *	@param		btn	a button code
 *	@returns	an event code
 **/
mode_event temperature_display_mode_t::handle(const button btn)
{
	switch (btn)
	{
	case BTN_LEFT:
	case BTN_RIGHT:
		_sensor_id = (_sensor_id == SENSOR_ID_INDOOR) ? SENSOR_ID_OUTDOOR : SENSOR_ID_INDOOR;
	default:
		break;
	}

	print_temperature();
	return ME_NONE;
}

/**
 *	Prints current temperature
 **/
void temperature_display_mode_t::print_temperature()
{
	int temp = (int)sensor.get_temp(_sensor_id);
	if(temp == _last_value)
	{
		return;
	}
	_last_value = temp;

	display.graphics().clear();
	display.graphics().clear_text();

	custom_char cc[4] = { CHAR_QUESTION, CHAR_QUESTION, CHAR_DEG, CHAR_C };

	cc[0] = display_t::to_custom_char(temp / 10);
	cc[1] = display_t::to_custom_char(temp % 10);
	display.print_g(cc);
	display.text().setCursor(0, 0);
	display.text().print("Temperature");

	display.text().setCursor(0, 1);
	switch (_sensor_id)
	{
	case SENSOR_ID_OUTDOOR:
		display.text().print("[outside]");
		break;
	case SENSOR_ID_INDOOR:
		display.text().print("[room]");
		break;
	default:
		break;
	}

	log.debug(F("temperature_display_mode\tprint_temperature(): t = %d"), temp);
}

/*
 ************************************************************************
 *	temperature_and_time_display_mode_t
 *	Temperature and time display app mode
 ************************************************************************
 */

/**
 *	Informs an app mode about activation
 **/
void temperature_and_time_display_mode_t::enter()
{
	_last_temp = 0;
	display.text().clear();
	print_temperature();
}

/**
 *	Handles empty button event
 **/
mode_event temperature_and_time_display_mode_t::handle()
{
	print_temperature();
	return ME_NONE;
}

/**
 *	Handles button event
 *	@param		btn	a button code
 *	@returns	an event code
 **/
mode_event temperature_and_time_display_mode_t::handle(const button btn)
{
	switch (btn)
	{
	case BTN_LEFT:
	case BTN_RIGHT:
		_sensor_id = (_sensor_id == SENSOR_ID_INDOOR) ? SENSOR_ID_OUTDOOR : SENSOR_ID_INDOOR;
	default:
		break;
	}

	print_temperature();
	return ME_NONE;
}

/**
 *	Prints current temperature
 **/
void temperature_and_time_display_mode_t::print_temperature()
{
	int temp = (int)(sensor.get_temp(_sensor_id) * 10);
	time_t time = time_service.get_time();
	if(temp != _last_temp)
	{
		display.text().setCursor(0, 0);
		display.text().print("Temp  ");
		if(temp >= 100)
		{
			display.text().print(util::get_number(temp, 2));
		}
		display.text().print(util::get_number(temp, 1));
		display.text().print('.');
		display.text().print(util::get_number(temp, 0));
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

		_last_temp = temp;
		log.debug(F("temperature_and_time_display_mode\tprint_temperature(): temp = %d"), temp);
	}

	if(time.h != _last_time.h ||
	   time.min != _last_time.min ||
	   time.sec != _last_time.sec)
	{
		display.text().setCursor(0, 1);
		display.text().print("Time ");
		util::print_number(display.text(), time.h, 2);
		display.text().print(':');
		util::print_number(display.text(), time.min, 2, '0');
		display.text().print(':');
		util::print_number(display.text(), time.sec, 2, '0');

		_last_time = time;
		log.debug(F("temperature_and_time_display_mode\tprint_temperature(): time = %d:%d:%d"), time.h, time.min, time.sec);
		return;
	}	
}

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
	