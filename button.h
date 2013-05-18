#pragma once

namespace thermograph
{
	/**
	 *	Button type enumeration
	 **/
	enum button
	{
		/**
		 *	No button has been pressed
		 **/
		BTN_NONE,

		/**
		 *	"LEFT" button has been pressed
		 **/
		BTN_LEFT,

		/**
		 *	"RIGHT" button has been pressed
		 **/
		BTN_RIGHT,

		/**
		 *	"UP" button has been pressed
		 **/
		BTN_UP,

		/**
		 *	"DOWN" button has been pressed
		 **/
		BTN_DOWN,

		/**
		 *	"SELECT" button has been pressed
		 **/
		BTN_SELECT
	};

	/**
	 *	Keypad adapter class
	 **/
	class button_service_t
	{
	public:
		/**
		*	Reads current button state
		 *	@returns button currently pressed
		 **/
		button read_button() const;

	private:
		/**
		 *	Reads current button state immediately
		 *	@returns button currently pressed
		 **/
		button read_button_internal() const;
	};

	/**
	 *	Keypad adapter static instance
	 **/
	extern button_service_t button_service;
}