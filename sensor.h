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
	typedef float temperature_t;

	/**
	*	Humidity
	*/
	typedef float humidity_t;

	/**
	*	A wrapper class for optional values
	*/
	template<typename T>
	class optional_t
	{
	public:
		/**
		*	Gets a wrapped value
		*	@returns	a wrapped value if it's set, a default value of T otherwise
		*/
		const T& value() const { return _value; }

		/**
		*	Gets a value indicating whether a value is set or not
		*	@return	true if value is set, false otherwise
		*/
		const bool has_value() const { return _has_value; }

		/**
		*	Factory method to create a value's wrapper
		*	@param		value	a value to wrap
		*	@returns	a wrapper for specified value
		*/
		static optional_t<T> create(const T& value)
		{
			optional_t<T> wrapper;
			wrapper._has_value = true;
			wrapper._value = value;
			return wrapper;
		}

		/**
		*	Factory method to create a empty wrapper
		*	@returns	an empty wrapper
		*/
		static optional_t<T> empty()
		{
			optional_t<T> wrapper;
			wrapper._has_value = false;
			return wrapper;
		}

	private:
		T	 _value;
		bool _has_value;
	};

	/**
	*	Sensor's readings
	*/
	class reading_t
	{
	public:
		/**
		*	Constructor
		*	@param	temperature	temperature's reading
		*	@param	humidity	humidity's reading
		*/
		reading_t(const optional_t<temperature_t>& temperature, const optional_t<humidity_t>& humidity);

		/**
		*	Gets a temperature's reading
		*	@returns	a temperature's reading
		*/
		const optional_t<temperature_t>& temperature() const { return _temperature; }

		/**
		*	Gets a humidity's reading
		*	@returns	a humidity's reading
		*/
		const optional_t<humidity_t>& humidity() const { return _humidity; }

	private:
		const optional_t<temperature_t> _temperature;
		const optional_t<humidity_t> _humidity;
	};

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
		virtual const reading_t update(const time_t& time) = 0;
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
		virtual const reading_t update(const time_t& time);

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
		static temperature_t approximate_temperature(double r, size_t index);
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
		virtual const reading_t update(const time_t& time);

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
		virtual const reading_t update(const time_t& time);

	private:
		/**
		*	DHT sensor adapter
		**/
		DHT _dht;
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
		*	Retreives a sensor's last temperature value
		*	@param		id	sensor's id
		*	@returns	sensor's last value
		**/
		const optional_t<temperature_t> get_temperature(sensor_id id) const;

		/**
		*	Retreives a sensor's last humidity value
		*	@param		id	sensor's id
		*	@returns	sensor's last value
		**/
		const optional_t<humidity_t> get_humidity(sensor_id id) const;

	private:
		/**
		*	Temperature and humidity source base class
		**/
		class source_t
		{
		public:
			/**
			*	Destructor
			**/
			virtual ~source_t() { }

			/**
			*	Initializes temperature sensor
			**/
			virtual void init();

			/**
			*	Retreives sensor's last temperature
			**/
			const optional_t<temperature_t> get_temperature() const { return _last_temperature; }

			/**
			*	Retreives sensor's last humidity
			**/
			const optional_t<humidity_t> get_humidity() const { return _last_humidity; }

			/**
			*	Updates sensor value
			*	@param	time	current global time
			**/
			void update(time_t& time);

			/**
			*	Returns a reference to the sensor assiciated with current source
			*	@returns	a reference to the sensor
			**/
			virtual sensor_node_t& get_sensor() = 0;

		private:
			/**
			* Last temperature
			**/
			optional_t<temperature_t> _last_temperature;

			/**
			* Last humidity
			**/
			optional_t<humidity_t> _last_humidity;
		};

		/**
		* Outdoor source
		**/
		class outdoor_source_t : public source_t
		{
		public:
			/**
			*	Constructor
			*	@param	port	sensor port
			**/
			outdoor_source_t(int port) : _sensor(port, DHT_11) { }

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
		*	Indoor source
		**/
		class indoor_source_t : public source_t
		{
		public:
			/**
			*	Constructor
			*	@param	port	sensor port
			**/
			indoor_source_t(int port) : _sensor(port, DHT_11) { }

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
		*	Indoor source
		**/		
		indoor_source_t _indoor_source;

		/**
		*	Outdoor source
		**/
		outdoor_source_t _outdoor_source;

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

		/**
		*	Writes current readings into the log
		*/
		void log_readings();
	};

	/**
	*	Sensor service static instance
	**/
	extern sensor_service_t sensor;
}