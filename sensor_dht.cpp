#include "Arduino.h"
#include "data_history.h"
#include "sensor.h"
#include "log.h"

using namespace thermograph;

#pragma region dht_sensor_node_t

/*
 ************************************************************************
 *	dht_sensor_node_t
 *	DHT11/22 sensor graph node
 ************************************************************************
 */

/**
 *	Constructor
 *	@param	port	sensor port
 *	@param	type	sensor type
 **/
dht_sensor_node_t::dht_sensor_node_t(uint8_t port, dht_sensor_type type)
	: _dht(port, type == DHT_11 ? DHT11 : DHT22)
{
}

/**
 *	Initializes a sensor node
 **/
void dht_sensor_node_t::init()
{
	_dht.begin();
	log.debug(F("dht_sensor\tinit()"));
}

/**
 *	Retreives node's current value
 *	@param		time	current global time
 *	@returns	node's temperature value
 **/
const reading_t dht_sensor_node_t::update(const time_t& time)
{
	float humidity = _dht.readHumidity();
	float temperature= _dht.readTemperature();

	if(isnan(humidity) || isnan(temperature))
	{
		log.error(F("dht_sensor\tupdate(): unable to read data from sensor"));
		return reading_t(
			optional_t<temperature_t>::empty(),
			optional_t<humidity_t>::empty()
			);
	}

	log.debug(F("dht_sensor\tupdate(): temperature = %f deg C, humidity = %f%%"), &temperature, &humidity);
	return reading_t(
		optional_t<temperature_t>::create(temperature),
		optional_t<humidity_t>::create(humidity)
		);
}

#pragma endregion