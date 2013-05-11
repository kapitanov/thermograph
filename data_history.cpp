#include "Arduino.h"
#include "_config.h"
#include "data_history.h"
#include "log.h"
#include "LCDBitmap.h"

using namespace thermograph;

data_history_t thermograph::data_history;

void data_history_t::push(const time_t& time, const temp_t t)
{
	if(_is_first)
	{
		for (size_t i = 0; i < DATA_POINTS_COUNT; i++)
		{
			_points[i - 1].time =time;
			_points[i - 1].value =  t;
		}

		_is_first = false;
	}
	else
	{
		time_t& last_time = _points[DATA_POINTS_COUNT - 1].time;
		long delta = 
			time.sec - last_time.sec +
			(time.min - last_time.min) * 60 +
			(time.h - last_time.h) * 360;

		if(delta < APP_HISTORY_INTERVAL)
		{
			return;
		}

		for (size_t i = 1; i < DATA_POINTS_COUNT; i++)
		{
			_points[i - 1].time = _points[i].time;
			_points[i - 1].value =  _points[i].value;
		}

		data_point_t& point = _points[DATA_POINTS_COUNT - 1];
		point.time = time;
		point.value = t;
	}

	_rev++;

	log.info(F("data_history\tpush(): new data point, t = %f deg, rev = #%d"), &t, _rev);
}


void data_history_t::get_points(byte* arr) const
{
#ifdef APP_CHART_MODE_AVG
	float ps[DATA_POINTS_COUNT];

	// Dump measurement points & calculate boundaries
	float min = 1000, max = -1000, avg = 0;
	for (int i = 0; i < DATA_POINTS_COUNT; i++)
	{
		float x = _points[i].value;
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
		int x = _points[i].value;
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