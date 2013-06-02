#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include "_config.h"
#include "time.h"
#include "DHT.h"

namespace thermograph
{
	/**
	 *	Temperature
	 */
	typedef float temp_t;

	/**
	 *	Sensor graph node
	 **/
	class sensor_node_t
	{
	public:
		/**
		 *	Destructor
		 **/
		virtual ~sensor_node_t() { }

		/**
		 *	Initializes a sensor node
		 **/
		virtual void init() { }

		/**
		 *	Retreives node's current value
		 *	@param		time	current global time
		 *	@returns	node's temperature value
		 **/
		virtual const temp_t update(const time_t& time) = 0;
	};

	/**
	 *	Thermistor sensor graph node
	 **/
	class thermistor_sensor_node_t : public sensor_node_t
	{
	public:
		/**
		 *	Constructor
		 *	@param	port	sensor port
		 **/
		thermistor_sensor_node_t(uint8_t port)
			: _port(port)
		{ }

		/**
		 *	Retreives node's current value
		 *	@param		time	current global time
		 *	@returns	node's temperature value
		 **/
		virtual const temp_t update(const time_t& time);

		/**
		 *	Temperature point for approximation
		 **/
		struct temperature_point_t
		{
			/**
			 *	Temperature
			 **/
			double t;

			/**
			 *	Resistance
			 **/
			double r;
		};

	private:
		/**
		 *	Sensor port
		 **/
		uint8_t _port;

		/**
		 *	Thermistor voltage dividor resistor's resistance
		 **/
		static const double R_D;

		/**
		 *	Thermistor's resistance at 25 Celsium degrees
		 **/
		static const double R_25;
		
		/**
		 *	Finds a closest temperature point
		 *	@param		r	thermistor's resistance
		 *	@returns	an index of temperature point, -1 if no temperature point has been found
		 **/
		static size_t find_temperature_point(double r);

		/**
		 *	Computes temperature
		 *	@param		r		thermistor's resistance
		 *	@param		index	temperature point's index
		 *	@returns	temperature
		 **/
		static temp_t approximate_temperature(double r, size_t index);
	};

	/**
	 *	LM35 sensor graph node
	 **/
	class lm35_sensor_node_t : public sensor_node_t
	{
	public:
		/**
		 *	Constructor
		 *	@param	port	sensor port
		 **/
		lm35_sensor_node_t(uint8_t port)
			: _port(port)
		{ }

		/**
		 *	Initializes a sensor node
		 **/
		virtual void init();

		/**
		 *	Retreives node's current value
		 *	@param		time	current global time
		 *	@returns	node's temperature value
		 **/
		virtual const temp_t update(const time_t& time);

	private:
		/**
		 *	Sensor port
		 **/
		uint8_t _port;
	};

	/**
	 *	DHT sensor type
	 **/
	enum dht_sensor_type
	{
		/**
		 *	DHT11
		 **/
		DHT_11,

		/**
		 *	DHT22
		 **/
		DHT_22
	};

	/**
	 *	DHT11/DHT22 sensor graph node
	 **/
	class dht_sensor_node_t : public sensor_node_t
	{
	public:
		/**
		 *	Constructor
		 *	@param	port	sensor port
		 *	@param	type	sensor type
		 **/
		dht_sensor_node_t(uint8_t port, dht_sensor_type type);

		/**
		 *	Initializes a sensor node
		 **/
		virtual void init();

		/**
		 *	Retreives node's current value
		 *	@param		time	current global time
		 *	@returns	node's temperature value
		 **/
		virtual const temp_t update(const time_t& time);

	private:
		/**
		 *	DHT sensor adapter
		 **/
		DHT _dht;
	};


	/**
	 *	Temperature value's filter sensor graph node
	 **/
	class filter_sensor_node_t : public sensor_node_t
	{
	public:
		/**
		 *	Constructor
		 *	@param	source	source graph node
		 **/
		filter_sensor_node_t(sensor_node_t& source)
			: _source(&source),
			  _is_first_step(true)
		{ }

		/**
		 *	Constructor
		 **/
		filter_sensor_node_t()
			: _source(NULL),
			  _is_first_step(true)
		{ }

		/**
		 *	Attaches filter to the sensor graph node
		 *	@param	source	source graph node
		 **/
		void attach_to(sensor_node_t& source);

		/**
		 *	Retreives node's current value
		 *	@param		time	current global time
		 *	@returns	node's temperature value
		 **/
		virtual const temp_t update(const time_t& time);

	private:
		/**
		 *	Source graph node
		 **/
		sensor_node_t* _source;

		/**
		 *	Filter's kernel size
		 **/
		static const size_t FILTER_KERNEL_LENGTH = 3;

		/**
		 *	Filter's kernel
		 **/
		temp_t _kernel[FILTER_KERNEL_LENGTH];

		/**
		 *	Last temperature value
		 **/
		temp_t _last_value;

		/**
		 *	Indicates whether a filter has been initialized
		 **/
		bool _is_first_step;
	};

	/**
	 *	Sensor ID
	 **/
	enum sensor_id
	{
		/**
		 *	Outdoor sensor
		 **/
		SENSOR_ID_OUTDOOR,

		/**
		 *	Indoor sensor
		 **/
		SENSOR_ID_INDOOR
	};

	/**
	 *	Sensor service class
	 **/
	class sensor_service_t
	{
	public:
		/**
		 *	Constructor
		 **/
		sensor_service_t();

		/**
		 *	Sensor update policy enumeration
		 **/
		enum update_policy
		{
			/**
			 *	Update sensor values by timer
			 **/
			UP_DEFAULT,

			/**
			 *	Force sensor values update
			 **/
			UP_FORCE
		};

		/**
		 *	Initializes sensors
		 **/
		void init();

		/**
		 *	Updates sensor values
		 *	@param	policy	sensor update policy
		 **/
		void update(update_policy policy = UP_DEFAULT);

		/**
		 *	Retreives a sensor's last value
		 *	@param		id	sensor's id
		 *	@returns	sensor's last value
		 **/
		const temp_t get_temp(sensor_id id) const;

	private:
		/**
		 *	Temperature source base class
		 **/
		class temperature_source_t
		{
		public:
			/**
			 *	Destructor
			 **/
			virtual ~temperature_source_t() { }

			/**
			 *	Initializes temperature sensor
			 **/
			virtual void init();

			/**
			 *	Retreives sensor's last temperature
			 **/
			const temp_t get_temp() const { return _last_temp; }

			/**
			 *	Updates sensor value
			 *	@param	time	current global time
			 **/
			void update(time_t& time);

		protected:
#ifdef APP_ENABLE_FILTER
			/**
			 *	Constructor
			 **/
			temperature_source_t() : _filter() { }
#endif
			/**
			 *	Returns a reference to the sensor assiciated with current source
			 *	@returns	a reference to the sensor
			 **/
			virtual sensor_node_t& get_sensor() = 0;

		private:
#ifdef APP_ENABLE_FILTER
			/**
			 *	Temperature sensor values filters
			 **/
			filter_sensor_node_t _filter;
#endif

			/**
			 * Last temperature
			 **/
			temp_t _last_temp;
		};

		/**
		 * Outdoor temperature source
		 **/
		class outdoor_temperature_source_t : public temperature_source_t
		{
		public:
			/**
			 *	Constructor
			 *	@param	port	sensor port
			 **/
			outdoor_temperature_source_t(int port) : _sensor(port) { }
			
		protected:
			/**
			 *	Returns a reference to the sensor assiciated with current source
			 *	@returns	a reference to the sensor
			 **/
			virtual sensor_node_t& get_sensor() { return _sensor; }

		private:
			/**
			 *	Temperature sensor
			 **/
			thermistor_sensor_node_t _sensor;
		};

		/**
		 *	Indoor temperature source
		 **/
		class indoor_temperature_source_t : public temperature_source_t
		{
		public:
			/**
			 *	Constructor
			 *	@param	port	sensor port
			 **/
			indoor_temperature_source_t(int port) : _sensor(port, DHT_11) { }
			
		protected:
			/**
			 *	Returns a reference to the sensor assiciated with current source
			 *	@returns	a reference to the sensor
			 **/
			virtual sensor_node_t& get_sensor() { return _sensor; }

		private:
			/**
			 *	Temperature sensor
			 **/
			dht_sensor_node_t _sensor;
		};

		/**
		 *	Indoor temperature source
		 **/		
		indoor_temperature_source_t _indoor_source;

		/**
		 *	Outdoor temperature source
		 **/
		outdoor_temperature_source_t _outdoor_source;
		
		/**
		 *	Last update time
		 **/
		sys_time_t _last_updated;

		/**
		 *	Determines whether sensor values should be updated
		 *	@param		time	current local time
		 *	@param		policy	sensor value update policy
		 *	@returns	a value indicating whether sensor values should be updated or not.
		 **/
		bool execute_policy(const sys_time_t& time, update_policy policy);
	};

	/**
	 *	Sensor service static instance
	 **/
	extern sensor_service_t sensor;
}