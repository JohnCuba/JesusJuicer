#include <Arduino.h>
#include "ArduinoJson.h"

#pragma once

struct wifiCredentials {
	String ssid;
	String password;
};

void convertFromJson(JsonVariantConst src, wifiCredentials& dst);
void convertToJson(wifiCredentials src, JsonVariant variant);
