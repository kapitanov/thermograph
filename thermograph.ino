#include "app.h"

thermograph::app_t app;

void setup()
{
	app.init();
}

void loop()
{
	app.run();
}
