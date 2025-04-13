#include "logg.hpp"

#pragma once

class PHSensorModule
{
private:
	static PHSensorModule *pinstance_;
	Logg logg = Logg("PH");
	static const int16_t sensorPin = 34;
	uint16_t analogValue = 0.0;
	float voltage = 0.0;
	float ph = 0.0;

protected:
	PHSensorModule() {};
	~PHSensorModule() {};

public:
	static PHSensorModule *GetInstance();
	void onSetup();
	float getValue();
};
