#include "Arduino.h"
#include "button.h"

using namespace thermograph;

button_service_t thermograph::button_service;

button button_service_t::read_button() const
{
	button btn = read_button_internal();
	if(btn != BTN_NONE)
	{
		delay(250);
	}

	return btn;
}

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