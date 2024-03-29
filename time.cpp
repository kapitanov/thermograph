#include "Arduino.h"
#include "time.h"

using namespace thermograph;

/**
 *	Time service static instance
 **/
thermograph::time_service_t thermograph::time_service;

/*
 ************************************************************************
 *	time_service_t
 *	Time service class
 ************************************************************************
 */

/**
 *	Gets current global time
 *	@returns	current global time value
 **/
const time_t time_service_t::get_time() const
{
	time_t time;
	unsigned long sec = millis() / 1000;
	unsigned long min = sec / 60;
	time.sec = sec % 60;
	time.h = min / 60;
	time.min = min % 60;
	
	return time;
}

/**
 *	Time service static instance
 **/
const sys_time_t time_service_t::get_systime() const
{
	sys_time_t time;

	unsigned long ms = millis();
	unsigned long sec = ms / 1000;
	unsigned long min = sec / 60;
	unsigned long h = min / 60;
	sec = sec % 60;
	min = min % 60;
	ms = ms % 1000;

	time.h = h;
	time.min = min;
	time.sec = sec;
	time.ms = ms;
		
	return time;
}