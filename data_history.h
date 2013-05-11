#pragma once

#include "time.h"
#include "sensor.h"

namespace thermograph
{
	class data_history_t
	{
	public:
		static const int DATA_POINTS_COUNT = 20;
		
		data_history_t() : _is_first(true) { }

		void push(const time_t& time, const temp_t t);
		void get_points(byte* arr) const;

		int get_revision() const { return _rev; }

	private:
		
		struct data_point_t
		{
			temp_t value;
			time_t time;
		};

		data_point_t _points[DATA_POINTS_COUNT];
		int _rev;
		bool _is_first;
	};

	extern data_history_t data_history;
}