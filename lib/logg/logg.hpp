#include <Arduino.h>

#pragma once

class Logg {
	private:
	String moduleName;
	public:
	Logg(String moduleName);
	void info(String message);
};
