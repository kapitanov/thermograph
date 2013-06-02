#include "Arduino.h"
#include "data_history.h"
#include "sensor.h"
#include "log.h"

using namespace thermograph;

#pragma region thermistor_sensor_node_t

/*
 ************************************************************************
 *	thermistor_sensor_node_t
 *	Thermistor sensor graph node
 ************************************************************************
 */

/**
 *	Thermistor voltage dividor resistor's resistance
 **/
const double thermistor_sensor_node_t::R_D = 1.0*1000;

/**
 *	Thermistor's resistance at 25 Celsium degrees
 **/
const double thermistor_sensor_node_t::R_25 = 10.0*1000;

/**
 *	Thermistor's approximation data
 **/
static thermistor_sensor_node_t::temperature_point_t temperature_points[] =
{
	{ -55.0, 121.46 },
	{ -50.0, 84.439 },
	{ -45.0, 59.243 },
	{ -40.0, 41.938 },
	{ -35.0, 29.947 },

	{ -30.0, 21.567 },
	{ -25.0, 15.641 },
	{ -20.0, 11.466 },
	{ -15.0, 8.451 },
	{ -10.0, 6.2927 },

	{  -5.0, 4.7077 },
	{   0.0, 3.5563 },
	{   5.0, 2.7119 },
	{  10.0, 2.086  },
	{  15.0, 1.6204 },

	{  20.0, 1.2683 },
	{  25.0, 1.0000 },
	{  30.0, 0.7942 },
	{  35.0, 0.63268 },

	{  40.0, 0.5074 },
	{  45.0, 0.41026 },
	{  50.0, 0.33363 },
	{  55.0, 0.27243 }
};

/**
 *	Finds a closest temperature point
 *	@param		r	thermistor's resistance
 *	@returns	an index of temperature point, -1 if no temperature point has been found
 **/
size_t thermistor_sensor_node_t::find_temperature_point(double r)
{
	size_t count = sizeof(temperature_points) / sizeof(thermistor_sensor_node_t::temperature_point_t);

	for(size_t i = 1; i < count; i++)
	{
		if(temperature_points[i].r <= r)
		{
			return i - 1;
		}
	}

	return -1;
}

/**
 *	Computes temperature
 *	@param		r		thermistor's resistance
 *	@param		index	temperature point's index
 *	@returns	temperature
 **/
temperature_t thermistor_sensor_node_t::approximate_temperature(double r, size_t index)
{
	const thermistor_sensor_node_t::temperature_point_t* p1 = &(temperature_points[index]);
	const thermistor_sensor_node_t::temperature_point_t* p2 = &(temperature_points[index + 1]);
	
	double a = (p2->t - p1->t) / (p2->r - p1->r);
	double b = p1->t - a * p1->r;

	double t = a*r + b;
	return static_cast<temperature_t>(t);
}

/**
 *	Retreives node's current value
 *	@param		time	current global time
 *	@returns	node's temperature value
 **/
const reading_t thermistor_sensor_node_t::update(const time_t& time)
{
	log_event_t e = log.begin_event(LOG_DEBUG);
	e.printf(F("thermistor_sensor\tupdate(%u) "), &time);

	int a = analogRead(_port);
	e.printf(F("A = %d; "), a);

	double a_t = a / 1023.0;
	e.printf(F("At = %F; "), &a_t);

	double r_t = R_D * (1.0 - a_t) / a_t;
	e.printf(F("Rt = %F Ohm; "), &r_t);

	double k = r_t / R_25;
	e.printf(F("Kt = %F; "), &k);

	size_t index = find_temperature_point(k);
	e.printf(F("i = %d; "), index);

	if(index < 0)
	{
		e.printf(F("temp. point not found"));
		return reading_t(
			optional_t<temperature_t>::empty(),
			optional_t<humidity_t>::empty()
			);
	}

	temperature_t t = approximate_temperature(k, index);
	e.printf(F("t = %f deg C"), &t);

	return reading_t(
		optional_t<temperature_t>::create(t),
		optional_t<humidity_t>::empty()
		);
}

#pragma endregion