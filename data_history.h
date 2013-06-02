#pragma once

#include "time.h"
#include "sensor.h"

namespace thermograph
{
	/**
	 *	Measurements' history storage class
	 **/
	class data_history_t
	{
	public:
		/**
		 *	Max data points to store
		 **/
		static const int DATA_POINTS_COUNT = 20;
		
		/**
		 *	Constructor
		 **/
		data_history_t() : _is_first(true) { }

		/**
		 *	Pushes a new value into the history
		 *	@param	id		sensor's ID
		 *	@param	time	current global time
		 *	@param	t		measurement result
		 **/
		void push(const sensor_id id, const time_t& time, const temperature_t t);

		/**
		 *	Retrieves last measurement results in normalized form.
		 *	Measurement results will be normalized to [0, 255] range
		 *	@param	id	sensor's ID
		 *	@param	arr	an array for measurement results. Must have at least DATA_POINTS_COUNT items
		 */
		void get_points(const sensor_id id, byte* arr) const;

		/**
		 *	Gets last history revision number
		 *	@returns	last history revision number
		 */
		int get_revision() const { return _rev; }

	private:
		
		/**
		 * A measurement data point
		 **/
		struct data_point_t
		{
			/**
			 *	Measured value
			 */
			temperature_t value;

			/**
			 *	Global tme of measurement
			 */
			time_t time;
		};

		/**
		 *	Measurement results for SENSOR_ID_INDOOR sensor
		 **/
		mutable data_point_t _indoor_points[DATA_POINTS_COUNT];

		/**
		 *	Measurement results for SENSOR_ID_OUTDOOR sensor
		 **/
		mutable data_point_t _outdoor_points[DATA_POINTS_COUNT];

		/**
		 *	Current revision number
		 */
		int _rev;

		/**
		 * Indicates whether history is empty
		 **/
		bool _is_first;

		/**
		 *	Gets an array containing measurement results for specified sensor
		 *	@param		id	sensor's ID
		 *	@returns	an array of measurement data points
		 **/
		data_point_t* get_array(const sensor_id id) const;
	};

	/**
	 *	Measurements' history storage static instance
	 **/
	extern data_history_t data_history;
}