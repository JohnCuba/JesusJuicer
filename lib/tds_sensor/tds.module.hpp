#include "logg.hpp"

#pragma once

class TDSModule {
	private:
	static TDSModule* pinstance_;
	Logg logg = Logg("TDS");
	static const int16_t sensorPin = 36;
	static const int16_t vRef = 3.3;
	static const int16_t sCount = 30;

	int analogBuffer[sCount];
	int analogBufferTemp[sCount];
	int analogBufferIndex = 0;
	int copyIndex = 0;

	float averageVoltage = 0;
	float rawTdsValue = 0;
	float temperature = 25;

	void registerServerRoutes();

	protected:
	TDSModule() {}
	~TDSModule() {}

	public:
	static TDSModule *GetInstance();
	void onSetup();
	float getValue();
};
