#include "Arduino.h"
#include "_config.h"
#include "data_history.h"
#include "log.h"
#include "LCDBitmap.h"

using namespace thermograph;

/**
 *	Measurements' history storage static instance
 **/
data_history_t thermograph::data_history;

/*
 ************************************************************************
 *	data_history_t
 *	Measurements' history storage class
 ************************************************************************
 */

/**
 *	Pushes a new value into the history
 *	@param	id		sensor's ID
 *	@param	time	current global time
 *	@param	t		measurement result
 **/
void data_history_t::push(const sensor_id id, const time_t& time, const temp_t t)
{
	data_point_t* points = get_array(id);

	if(_is_first)
	{
		// If history is empty then fill the history with current value
		for (size_t i = 0; i < DATA_POINTS_COUNT; i++)
		{
			points[i - 1].time =time;
			points[i - 1].value =  t;
		}

		_is_first = false;
	}
	else
	{
		time_t& last_time = points[DATA_POINTS_COUNT - 1].time;
		long delta = 
			time.sec - last_time.sec +
			(time.min - last_time.min) * 60 +
			(time.h - last_time.h) * 360;

		if(delta < APP_HISTORY_INTERVAL)
		{
			// Ignore value updates if it happened too fast
			return;
		}

		// Move measurement results in order to free the last array item
		for (size_t i = 1; i < DATA_POINTS_COUNT; i++)
		{
			points[i - 1].time = points[i].time;
			points[i - 1].value =  points[i].value;
		}

		// Write current value into the last array item
		data_point_t& point = points[DATA_POINTS_COUNT - 1];
		point.time = time;
		point.value = t;
	}

	// Increment revision number
	_rev++;

	log.info(F("data_history\tpush(): new data point, t = %f deg, rev = #%d"), &t, _rev);
}

/**
 *	Retrieves last measurement results in normalized form.
 *	Measurement results will be normalized to [0, 255] range
 *	@param	id	sensor's ID
 *	@param	arr	an array for measurement results. Must have at least DATA_POINTS_COUNT items
 */
void data_history_t::get_points(const sensor_id id, byte* arr) const
{
	const data_point_t* points = get_array(id);

#ifdef APP_CHART_MODE_AVG
	float ps[DATA_POINTS_COUNT];

	// Dump measurement points & calculate boundaries
	float min = 1000, max = -1000, avg = 0;
	for (int i = 0; i < DATA_POINTS_COUNT; i++)
	{
		float x = points[i].value;
		ps[i] = x;

		if(x > max)
		{
			max = x;
		} 
		if(x < min)
		{
			min = x;
		}
		avg += x;
	}

	// Normalize data
	avg /= DATA_POINTS_COUNT;
	float amp = max(avg - min, max - avg);
	float k = (BITMAP_H/2.0 - 1) / amp;

	log_event_t e = log.begin_event(LOG_DEBUG);
	e.printf(F("data_history\tget_points(): avg %f, amp %f, k = %f -> [ "), &avg, &amp, &k);

	for (int i = 0; i < DATA_POINTS_COUNT; i++)
	{
		float x = ps[i];
		float t = k*(x - avg);

		byte y = (byte)(k*(x - avg)) + BITMAP_H/2;
		arr[i] = y;
		e.printf(F("(%f, %d) "), &x, y);
	}

	e.printf(F("]"));
#else
	float ps[DATA_POINTS_COUNT];

	// Dump measurement points & calculate boundaries
	float max = -1000;
	for (int i = 0; i < DATA_POINTS_COUNT; i++)
	{
		int x = points[i].value;
		ps[i] = x;
		if(x > max)
		{
			max = x;
		}
	}

	// Normalize data
	float k = (BITMAP_H/2.0 - 1) / max;

	log_event_t e = log.begin_event(LOG_DEBUG);
	e.printf(F("data_history\tget_points(): to %f with k = %f [ "), &max, &k);

	for (int i = 0; i < DATA_POINTS_COUNT; i++)
	{
		float x = ps[i];
		byte y = (byte)(k*x) + 1;
		arr[i] = y;
		e.printf(F("(%f, %d) "), &x, y);
	}

	e.printf(F("]"));
#endif
}

/**
 *	Gets an array containing measurement results for specified sensor
 *	@param		id	sensor's ID
 *	@returns	an array of measurement data points
**/
data_history_t::data_point_t* data_history_t::get_array(const sensor_id id) const
{
	switch (id)
	{
	case SENSOR_ID_INDOOR:
		return _indoor_points;
	case SENSOR_ID_OUTDOOR:
		return _outdoor_points;
	default:
		return NULL;
	}
}