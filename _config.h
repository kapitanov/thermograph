#pragma once

/* 
 * Internal tempature sensor's port
 */
#define APP_INDOOR_SENSOR_PORT A1

/* 
 * External tempature sensor's port
 */
#define APP_OUTDOOR_SENSOR_PORT A2

/* 
 * Temperature sensors' update period, in milliseconds
 */
#define APP_SENSOR_PERIOD ((long)1000*10) /* ms */


/*
 * Measurement history write period, in seconds
 */
#define APP_HISTORY_INTERVAL ((long)60) /* sec */

/*
 * Chart display mode - using normalization to average.
 * Uses normalization to boundaries otherwise
 */
#define APP_CHART_MODE_AVG

/*
 * Delta neighborhood for measurements' filter
 */
#define APP_FILTER_DELTA 1.0

/*
 * Enable measurements' filter
 */
//#define APP_ENABLE_FILTER

/*
 * LCD display ports
 */
#define LCD_RS_PORT		8
#define LCD_ENABLE_PORT	9
#define LCD_D0_PORT		4
#define LCD_D1_PORT		5
#define LCD_D2_PORT		6
#define LCD_D3_PORT		7
#define LCD_BITMAP_X	0
#define LCD_BITMAP_Y	0
#define LCD_WIDTH		16
#define LCD_HEIGHT		2

/*
 * Enable precision mode for LM35 temperature sensor, using internal analog reference voltage.
 */
#define LM35_USE_INTERNAL_REF

/*
 * Enable debug logging level. Displays messages with DBG, INF, ERR levels.
 * Displays only IFN and ERR messages otherwise.
 */
#define ENABLE_DEBUG_LOGGING