#pragma once

#define APP_SENSOR_PORT A1
#define APP_SENSOR_PERIOD ((long)1000*10) /* ms */

#define APP_HISTORY_INTERVAL ((long)60) /* sec */
#define APP_CHART_MODE_AVG
#define APP_FILTER_DELTA 1.0

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

//#define ENABLE_DEBUG_LOGGING