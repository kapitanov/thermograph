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
	 *	Temperature display app mode
	 **/
	class temperature_display_mode_t : public mode_t
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
		 *	Prints current temperature
		 **/
		void print_temperature();
	};

	/**
	 *	Temperature and time display app mode
	 **/
	class temperature_and_time_display_mode_t : public mode_t
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
		int _last_temp;

		/**
		 *	Last displayed time
		 **/
		time_t _last_time;

		/**
		 *	Active sensor ID
		 **/
		sensor_id _sensor_id;

		/**
		 *	Prints current temperature
		 **/
		void print_temperature();
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