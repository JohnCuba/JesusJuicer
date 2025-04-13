#include "ph_sensor.module.hpp"

PHSensorModule *PHSensorModule::pinstance_{nullptr};

PHSensorModule *PHSensorModule::GetInstance()
{
	if (pinstance_ == nullptr)
	{
		pinstance_ = new PHSensorModule();
	}

	return pinstance_;
}

void PHSensorModule::onSetup()
{
	logg.info("start setup");
	pinMode(sensorPin, INPUT);
	logg.info("end setup");
}

float PHSensorModule::getValue()
{
	analogValue = analogRead(sensorPin);
	Serial.print(analogValue);
	Serial.print(" | ");
	voltage = analogValue * (3.3 / 4095.0);
	ph = (3.3 * voltage);
	Serial.println(ph);
	return ph;
}