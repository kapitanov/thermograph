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

