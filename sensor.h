#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include "time.h"

namespace thermograph
{
	typedef float temp_t;

	class sensor_node_t
	{
	public:
		virtual ~sensor_node_t() { }

		virtual const temp_t update(const time_t& time) = 0;
	};

	class thermistor_sensor_node_t : public sensor_node_t
	{
	public:
		thermistor_sensor_node_t(uint8_t port)
			: _port(port)
		{ }

		virtual const temp_t update(const time_t& time);

		struct temperature_point_t
		{
			double t;
			double r;
		};

	private:
		uint8_t _port;

		static const double R_D;
		static const double R_25;
		
		static size_t find_temperature_point(double r);
		static double approximate_temperature(double r, size_t index);
	};

	class filter_sensor_node_t : public sensor_node_t
	{
	public:
		filter_sensor_node_t(sensor_node_t& source)
			: _source(source),
			  _is_first_step(true)
		{ }

		virtual const temp_t update(const time_t& time);

	private:
		sensor_node_t& _source;
		static const size_t FILTER_KERNEL_LENGTH = 3;

		temp_t _kernel[FILTER_KERNEL_LENGTH];
		temp_t _last_value;
		bool _is_first_step;
	};

	class sensor_service_t
	{
	public:
		sensor_service_t();

		enum update_policy
		{
			UP_DEFAULT,
			UP_FORCE
		};

		void update(update_policy policy = UP_DEFAULT);
		temp_t get_temp() const;

	private:
		thermistor_sensor_node_t _sensor;
		filter_sensor_node_t _filter;
		sensor_node_t* _source;
		sys_time_t _last_updated;
		temp_t _last_temp;

		bool execute_policy(const sys_time_t& time, update_policy policy);
	};

	extern sensor_service_t sensor;
}