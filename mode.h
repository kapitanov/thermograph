#pragma once

#include "button.h"
#include "sensor.h"

namespace thermograph
{
	enum mode_event
	{
		ME_NONE,
		ME_SWITCH_MODE
	};

	class mode_t
	{
	public:
		virtual void init() { }
		virtual void enter() { }
		mode_event handle_events();
		
	protected:
		virtual mode_event handle() { }
		virtual mode_event handle(thermograph::button btn) { return ME_NONE; }
	};

	class temperature_display_mode_t : public mode_t
	{
	public:
		virtual void enter();

	protected:
		virtual mode_event handle();
		virtual mode_event handle(const button btn);

	private:
		void print_temperature();
		int _last_value;
	};

	class temperature_and_time_display_mode_t : public mode_t
	{
	public:
		virtual void enter();

	protected:
		virtual mode_event handle();
		virtual mode_event handle(const button btn);

	private:
		void print_temperature();
		int _last_temp;
		time_t _last_time;
	};

	class temperature_chart_display_mode_t : public mode_t
	{
	public:
		virtual void enter();

	protected:
		virtual mode_event handle();
		virtual mode_event handle(const button btn);

	private:
		void print_chart(bool force);
		int _last_rev;
	};
}