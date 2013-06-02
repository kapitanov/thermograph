#include "Arduino.h"
#include "data_history.h"
#include "sensor.h"
#include "log.h"

using namespace thermograph;

#pragma region lm35_sensor_node_t

/*
************************************************************************
*	lm35_sensor_node_t
*	LM35 sensor graph node
************************************************************************
*/

/**
*	Initializes a sensor node
**/
void lm35_sensor_node_t::init()
{
#ifdef LM35_USE_INTERNAL_REF
	analogReference(INTERNAL);
#endif
}

/**
*	Retreives node's current value
*	@param		time	current global time
*	@returns	node's temperature value
**/
const reading_t lm35_sensor_node_t::update(const time_t& time)
{
	int reading = analogRead(_port);
#ifdef LM35_USE_INTERNAL_REF
	temperature_t temp = reading / 9.31;
#else
	temp_t temp = (5.0 * reading* 100.0) / 1024;
#endif

	log.debug(F("lm35_sensor\tupdate(): raw = %d, temp = %f"), reading, &temp);
	return reading_t(
		optional_t<temperature_t>::create(temp),
		optional_t<humidity_t>::empty()
		);
}

#pragma endregion