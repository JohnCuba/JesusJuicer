#include <Arduino.h>
#pragma once

class APCredentials {
	public:
	String ssid;
	String password;
	APCredentials(String, String);

	bool isEmpty();
};
