#include "logg.hpp"

#pragma once

class TDSModule
{
private:
	static const char loggTag_[4];
	static const int16_t sensorPin = 36;
	static const int16_t vRef = 3.3;
	static const int16_t sCount = 30;

	static void registerServerRoutes();

public:
	static void onSetup();
	static float getValue();
};
