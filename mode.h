#pragma once

#include "button.h"
#include "sensor.h"

namespace thermograph
{
	/**
	 *	App mode event code enumeration
	 **/
	enum mode_event
	{
		/**
		 *	An empty event
		 **/
		ME_NONE,

		/**
		 *	User requested a app mode switch
		 **/
		ME_SWITCH_MODE
	};

	/**
	 *	App mode base class
	 **/
	class mode_t
	{
	public:
		/**
		 *	Destructor
		 **/
		virtual ~mode_t() { }

		/**
		 *	Initializes an app mode
		 **/
		virtual void init() { }

		/**
		 *	Informs an app mode about activation
		 **/
		virtual void enter() { }

		/**
		 *	Handles events
		 *	@returns	an event code
		 **/
		mode_event handle_events();
		
	protected:
		/**
		 *	Handles empty button event
		 **/
		virtual mode_event handle() { }

		/**
		 *	Handles button event
		 *	@param		btn	a button code
		 *	@returns	an event code
		 **/
		virtual mode_event handle(thermograph::button btn) { return ME_NONE; }
	};

	/**
	 *	A value to display
	 */
	enum value_id
	{
		/**
		 *	Temperature
		 */
		VALUE_TEMPERATURE,

		/**
		 *	Humidity
		 */
		VALUE_HUMIDITY
	};

	/**
	 *	Expanded display app mode
	 **/
	class expanded_display_mode_t : public mode_t
	{
	public:
		/**
		 *	Informs an app mode about activation
		 **/
		virtual void enter();

	protected:
		/**
		 *	Handles empty button event
		 **/
		virtual mode_event handle();

		/**
		 *	Handles button event
		 *	@param		btn	a button code
		 *	@returns	an event code
		 **/
		virtual mode_event handle(const button btn);

	private:
		/**
		 *	Last displayed temperature
		 **/
		int _last_value;

		/**
		 *	Active sensor ID
		 **/
		sensor_id _sensor_id;

		/**
		 *	Active value ID
		 */
		value_id _value_id;

		/**
		 *	Prints current value
		 **/
		void update_display();

		/**
		 *	Prints current temperature value
		 **/
		void print_temperature();

		/**
		 *	Prints current humidity value
		 **/
		void print_humidity();

		/**
		 *	Prints current sensor name
		 */
		void print_sensor_name();
	};

	/**
	 *	Condensed display app mode
	 **/
	class condensed_display_mode_t : public mode_t
	{
	public:
		/**
		 *	Informs an app mode about activation
		 **/
		virtual void enter();

	protected:
		/**
		 *	Handles empty button event
		 **/
		virtual mode_event handle();

		/**
		 *	Handles button event
		 *	@param		btn	a button code
		 *	@returns	an event code
		 **/
		virtual mode_event handle(const button btn);

	private:
		/**
		 *	Last displayed temperature
		 **/
		int _last_temperature;

		/**
		 *	Last displayed humidity
		 **/
		int _last_humidity;

		/**
		 *	Active sensor ID
		 **/
		sensor_id _sensor_id;

		/**
		 *	Prints current values
		 **/
		void print();
	};

	/**
	 *	Temperature chart app mode
	 **/
	class temperature_chart_display_mode_t : public mode_t
	{
	public:
		/**
		 *	Informs an app mode about activation
		 **/
		virtual void enter();

	protected:
		/**
		 *	Handles empty button event
		 **/
		virtual mode_event handle();

		/**
		 *	Handles button event
		 *	@param		btn	a button code
		 *	@returns	an event code
		 **/
		virtual mode_event handle(const button btn);

	private:
		/**
		 *	Last data history revision
		 **/
		int _last_rev;

		/**
		 *	Active sensor ID
		 **/
		sensor_id _sensor_id;
		
		/**
		 *	Prints a temperature plot
		 *	@param	force	disable data revision check
		 **/
		void print_chart(bool force);
	};
}