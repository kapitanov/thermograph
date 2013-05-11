#include "Arduino.h"
#include "_config.h"
#include "data_history.h"
#include "sensor.h"
#include "log.h"

using namespace thermograph;

sensor_service_t thermograph::sensor;


/*** thermistor_sensor_node_t ***/

const double thermistor_sensor_node_t::R_D = 1.0*1000;
const double thermistor_sensor_node_t::R_25 = 10.0*1000;

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

double thermistor_sensor_node_t::approximate_temperature(double r, size_t index)
{
	const thermistor_sensor_node_t::temperature_point_t* p1 = &(temperature_points[index]);
	const thermistor_sensor_node_t::temperature_point_t* p2 = &(temperature_points[index + 1]);

	//#ifdef DEBUG
	//	Serial.println(F("thermistor_sensor_t::approximate_temperature:"));
	//	Serial.print(F("between "));
	//	Serial.print(p1->t);
	//	Serial.print(F(" and "));
	//	Serial.print(p2->t);
	//	Serial.println(F(" deg"));
	//#endif

	double a = (p2->t - p1->t) / (p2->r - p1->r);
	double b = p1->t - a * p1->r;

	double t = a*r + b;
	return t;
}

const temp_t thermistor_sensor_node_t::update(const time_t& time)
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
		return -1;
	}

	temp_t t = (float)approximate_temperature(k, index);
	e.printf(F("t = %f deg C"), &t);

	return t;
}

/*** filter_sensor_node_t ***/

const temp_t filter_sensor_node_t::update(const time_t& time)
{
	temp_t t = _source.update(time);
	if(_is_first_step)
	{
		for(size_t i = 0; i < FILTER_KERNEL_LENGTH; i++)
		{
			_kernel[i] = t;
		}

		_last_value = t;
		_is_first_step = false;

		log.debug(F("filter_sensor_node\tupdate(): initial t = %f deg"), &_is_first_step);
	}
	else
	{
		for(size_t i = 1; i < FILTER_KERNEL_LENGTH; i++)
		{
			_kernel[i - 1] = _kernel[i];
		}

		_kernel[FILTER_KERNEL_LENGTH - 1] = t;

		temp_t avg = 0;
		for(size_t i = 0; i < FILTER_KERNEL_LENGTH; i++)
		{
			avg += _kernel[i];
		}

		avg /= FILTER_KERNEL_LENGTH;
		temp_t delta = abs(t - avg);

		if(delta >= APP_FILTER_DELTA)
		{
			_last_value = avg;
			log.debug(F("filter_sensor_node\tupdate(): t = %f deg"), &_is_first_step);
		}
	}

	return _last_value;
}

/*** sensor_service_t ***/

sensor_service_t::sensor_service_t()
	: _sensor(APP_SENSOR_PORT),
	  _filter(_sensor),
	  _source(&_sensor)
{ }

void sensor_service_t::update(update_policy policy)
{
	sys_time_t time = time_service.get_systime();
	if(execute_policy(time, policy))
	{
		_last_temp = _sensor.update(time_service.get_time());
		_last_updated = time;

		log.info(F("sensor_service\tupdate(): t = %f deg"), &_last_temp);

		data_history.push(time_service.get_time(), _last_temp);
	}
}

bool sensor_service_t::execute_policy(const sys_time_t& time, update_policy policy)
{
	switch (policy)
	{
	case thermograph::sensor_service_t::UP_DEFAULT:
		{
			long delta = 0;
			delta += time.ms - _last_updated.ms;
			delta += (time.sec - _last_updated.sec) * 1000;
			delta += (time.min - _last_updated.min) * 60 * 1000;

			return (delta >= APP_SENSOR_PERIOD);
		}

	case thermograph::sensor_service_t::UP_FORCE:
		return true;

	default:
		break;
	}
}

temp_t sensor_service_t::get_temp() const
{
	return _last_temp;
}