#pragma once

namespace thermograph
{
	enum button
	{
		BTN_NONE,
		BTN_LEFT,
		BTN_RIGHT,
		BTN_UP,
		BTN_DOWN,
		BTN_SELECT
	};

	class button_service_t
	{
	public:
		button read_button() const;
	private:
		button read_button_internal() const;
	};

	extern button_service_t button_service;
}