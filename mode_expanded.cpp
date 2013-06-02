#include "Arduino.h"
#include "button.h"
#include "data_history.h"
#include "display.h"
#include "log.h"
#include "mode.h"
#include "sensor.h"
#include "util.h"

using namespace thermograph;

#pragma region expanded_display_mode_t

/*
************************************************************************
*	expanded_display_mode_t
*	Expanded display app mode
************************************************************************
*/

/**
*	Informs an app mode about activation
**/
void expanded_display_mode_t::enter()
{
	_last_value = 0;
	display.text().clear();
	update_display();
}

/**
*	Handles empty button event
**/
mode_event expanded_display_mode_t::handle()
{
	update_display();
	return ME_NONE;
}

/**
*	Handles button event
*	@param		btn	a button code
*	@returns	an event code
**/
mode_event expanded_display_mode_t::handle(const button btn)
{
	switch (btn)
	{
	case BTN_LEFT:
	case BTN_RIGHT:
		_sensor_id = (_sensor_id == SENSOR_ID_INDOOR) ? SENSOR_ID_OUTDOOR : SENSOR_ID_INDOOR;
		break;

	case BTN_UP:
	case BTN_DOWN:
		_value_id = (_value_id == VALUE_HUMIDITY) ? VALUE_TEMPERATURE : VALUE_HUMIDITY;
		break;

	default:
		return ME_NONE;
	}

	{
		log_event_t e = log.begin_event(LOG_INFO);
		e.printf(F("expanded_display_mode\thandle(const button): active sensor = "));
		switch (_sensor_id)
		{
		case SENSOR_ID_OUTDOOR:
			e.printf(F("SENSOR_ID_OUTDOOR"));
			break;
		case SENSOR_ID_INDOOR:
			e.printf(F("SENSOR_ID_INDOOR"));
			break;
		default:
			break;
		}

		e.printf(F(", active value = "));
		switch (_value_id)
		{
		case VALUE_TEMPERATURE:
			e.printf(F("VALUE_TEMPERATURE"));
			break;
		case VALUE_HUMIDITY:
			e.printf(F("VALUE_HUMIDITY"));
			break;
		default:
			break;
		}
	}

	_last_value = 0;
	update_display();
	return ME_NONE;
}

/**
*	Prints current temperature
**/
void expanded_display_mode_t::update_display()
{
	switch (_value_id)
	{
	case VALUE_TEMPERATURE:
		print_temperature();
		break;
	case VALUE_HUMIDITY:
		print_humidity();
		break;
	}
}

/**
*	Prints current sensor name
*/
void expanded_display_mode_t::print_sensor_name()
{
	display.text().setCursor(0, 1);
	switch (_sensor_id)
	{
	case SENSOR_ID_OUTDOOR:
		display.text().print("outside");
		break;
	case SENSOR_ID_INDOOR:
		display.text().print("in the room");
		break;
	default:
		break;
	}
}

/**
*	Prints current temperature value
**/
void expanded_display_mode_t::print_temperature()
{
	optional_t<temperature_t> temperature = sensor.get_temperature(_sensor_id);
	if(temperature.has_value() && temperature.value() == _last_value)
	{
		return;
	}
	
	display.graphics().clear();
	display.graphics().clear_text();

	custom_char cc[4] = { CHAR_QUESTION, CHAR_QUESTION, CHAR_DEG, CHAR_C };
	if(temperature.has_value())
	{
		int value = temperature.value();
		cc[0] = display_t::to_custom_char(value / 10);
		cc[1] = display_t::to_custom_char(value % 10);
	}

	display.print_g(cc);
	display.text().setCursor(0, 0);
	display.text().print("Temperature");

	print_sensor_name();

	log_event_t e = log.begin_event(LOG_DEBUG);
	e.printf(F("temperature_display_mode\tprint_temperature(): temperature = "));
	if(temperature.has_value())
	{
		int v = temperature.value();
		e.printf(F("%d deg C"), v);
		_last_value = v;
	}
	else
	{
		e.printf(F("<N/A>"));
	}
}

/**
*	Prints current humidity value
**/
void expanded_display_mode_t::print_humidity()
{
	optional_t<humidity_t> humidity = sensor.get_humidity(_sensor_id);
	if(humidity.has_value() && humidity.value() == _last_value)
	{
		return;
	}

	display.graphics().clear();
	display.graphics().clear_text();

	custom_char cc[4] = { CHAR_QUESTION, CHAR_QUESTION, CHAR_QUESTION, CHAR_PERCENT };
	if(humidity.has_value())
	{
		int value = humidity.value();
		cc[0] = display_t::to_custom_char(value / 100);
		if(cc[0] == CHAR_0)
		{
			cc[0] = CHAR_NONE;
		}
		value = value % 100;
		cc[1] = display_t::to_custom_char(value / 10);
		cc[2] = display_t::to_custom_char(value % 10);
	}

	display.print_g(cc);
	display.text().setCursor(0, 0);
	display.text().print("Humidity");

	print_sensor_name();

	log_event_t e = log.begin_event(LOG_DEBUG);
	e.printf(F("temperature_display_mode\tprint_temperature(): humidity = "));
	if(humidity.has_value())
	{
		int v = humidity.value();
		e.printf(F("%d%%"), v);
		_last_value = v;
	}
	else
	{
		e.printf(F("<N/A>"));
	}
}

#pragma endregion