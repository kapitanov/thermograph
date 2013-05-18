#include "Arduino.h"
#include "button.h"

using namespace thermograph;

/**
 *	Keypad adapter static instance
 **/
button_service_t thermograph::button_service;

/*
 ************************************************************************
 *	button_service_t
 *	Keypad adapter class
 ************************************************************************
 */

/**
 *	Read current button state
 *	@returns button currently pressed
 **/
button button_service_t::read_button() const
{
	button btn = read_button_internal();
	if(btn != BTN_NONE)
	{
		// Delay for 250ms if any button has been pressed
		delay(250);
	}

	return btn;
}

/**
 *	Reads current button state immediately
 *	@returns button currently pressed
 **/
button button_service_t::read_button_internal() const
{
	int adc_key_in = analogRead(0);

	if (adc_key_in < 50)   return BTN_RIGHT;
	if (adc_key_in < 195)  return BTN_UP;
	if (adc_key_in < 380)  return BTN_DOWN;
	if (adc_key_in < 555)  return BTN_LEFT;
	if (adc_key_in < 790)  return BTN_SELECT;

	return BTN_NONE;
}