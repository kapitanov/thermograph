#include "Arduino.h"
#include "data_history.h"
#include "sensor.h"
#include "log.h"

using namespace thermograph;

/**
 *	Sensor service static instance
 **/
sensor_service_t thermograph::sensor;

#pragma region reading_t

/*
 ************************************************************************
 *	reading_t
 *	Sensor's readings
 ************************************************************************
 */

/**
 *	Constructor
 *	@param	temperature	temperature's reading
 *	@param	humidity	humidity's reading
 */
reading_t::reading_t(const optional_t<temperature_t>& temperature, const optional_t<humidity_t>& humidity)
	: _temperature(temperature), _humidity(humidity)
{ }

#pragma endregion

#pragma region sensor_service_t::temperature_source_t

/*
 ************************************************************************
 *	sensor_service_t::temperature_source_t
 *	Temperature source base class
 ************************************************************************
 */

/**
 *	Initializes temperature sensor
 **/
void sensor_service_t::source_t::init()
{
	get_sensor().init();
}

/**
 *	Updates sensor value
 *	@param	time	current global time
 **/
void sensor_service_t::source_t::update(time_t& time)
{
	reading_t reading = get_sensor().update(time);
	_last_temperature = reading.temperature();
	_last_humidity = reading.humidity();
}

#pragma endregion

#pragma region sensor_service_t

/*
 ************************************************************************
 *	sensor_service_t
 *	Sensor service class
 ************************************************************************
 */

/**
 *	Constructor
 **/
sensor_service_t::sensor_service_t()
	: _indoor_source(APP_INDOOR_SENSOR_PORT),
	  _outdoor_source(APP_OUTDOOR_SENSOR_PORT)
{ }

/**
 *	Initializes sensors
 **/
void sensor_service_t::init()
{
	log.debug(F("sensor_service\tinit()"));

	_indoor_source.init();
	_outdoor_source.init();
}

/**
 *	Updates sensor values
 *	@param	policy	sensor update policy
 **/
void sensor_service_t::update(update_policy policy)
{
	sys_time_t time = time_service.get_systime();
	if(execute_policy(time, policy))
	{
		time_t time = time_service.get_time();
		_indoor_source.update(time);
		_outdoor_source.update(time);
		_last_updated = time_service.get_systime();

		log_readings();

		optional_t<temperature_t> outdoor_temperature = _outdoor_source.get_temperature();
		if(outdoor_temperature.has_value())
		{
			data_history.push(SENSOR_ID_OUTDOOR, time, outdoor_temperature.value());
		}

		optional_t<temperature_t> indoor_temperature = _indoor_source.get_temperature();
		if(indoor_temperature.has_value())
		{
			data_history.push(SENSOR_ID_INDOOR, time, indoor_temperature.value());
		}
	}
}

/**
 *	Determines whether sensor values should be updated
 *	@param		time	current local time
 *	@param		policy	sensor value update policy
 *	@returns	a value indicating whether sensor values should be updated or not.
 **/
bool sensor_service_t::execute_policy(const sys_time_t& time, update_policy policy)
{
	switch (policy)
	{
	case thermograph::sensor_service_t::UP_DEFAULT:
		{
			long delta = 0;
			delta += (time.sec - _last_updated.sec);
			delta += (time.min - _last_updated.min) * 60;

			return (delta >= APP_SENSOR_PERIOD);
		}

	case thermograph::sensor_service_t::UP_FORCE:
		return true;

	default:
		break;
	}
}

/**
*	Writes current readings into the log
*/
void sensor_service_t::log_readings()
{
	log_event_t e = log.begin_event(LOG_INFO);
	e.printf(F("sensor_service\tupdate(): "));

	optional_t<temperature_t> indoor_temperature = _indoor_source.get_temperature();
	optional_t<humidity_t> indoor_humidity = _indoor_source.get_humidity();
	e.printf(F("indoor: "));

	if(indoor_temperature.has_value())
	{
		temperature_t t = indoor_temperature.value();
		e.printf(F("t = %f deg C, "), &t);
	}
	else
	{
		e.printf(F("t = <N/A>, "));
	}

	if(indoor_humidity.has_value())
	{
		humidity_t h = indoor_humidity.value();
		e.printf(F("h = %f%%, "), &h);
	}
	else
	{
		e.printf(F("h = <N/A>, "));
	}

	optional_t<temperature_t> outdoor_temperature = _outdoor_source.get_temperature();
	optional_t<humidity_t> outdoor_humidity = _outdoor_source.get_humidity();
	e.printf(F("outdoor: "));

	if(outdoor_temperature.has_value())
	{
		temperature_t t = outdoor_temperature.value();
		e.printf(F("t = %f deg C, "), &t);
	}
	else
	{
		e.printf(F("t = <N/A>, "));
	}

	if(outdoor_humidity.has_value())
	{
		humidity_t h = outdoor_humidity.value();
		e.printf(F("h = %f%%"), &h);
	}
	else
	{
		e.printf(F("h = <N/A>"));
	}
}

/**
*	Retreives a sensor's last temperature value
*	@param		id	sensor's id
*	@returns	sensor's last value
**/
const optional_t<temperature_t> sensor_service_t::get_temperature(sensor_id id) const
{
	switch (id)
	{
	case SENSOR_ID_OUTDOOR:
		return _outdoor_source.get_temperature();
	case SENSOR_ID_INDOOR:
		return _indoor_source.get_temperature();
	default:
		break;
	}
}

/**
*	Retreives a sensor's last humidity value
*	@param		id	sensor's id
*	@returns	sensor's last value
**/
const optional_t<humidity_t> sensor_service_t::get_humidity(sensor_id id) const
{
	switch (id)
	{
	case SENSOR_ID_OUTDOOR:
		return _outdoor_source.get_humidity();
	case SENSOR_ID_INDOOR:
		return _indoor_source.get_humidity();
	default:
		break;
	}
}

#pragma endregion