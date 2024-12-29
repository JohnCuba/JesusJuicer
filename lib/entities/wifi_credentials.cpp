#include "wifi_credentials.hpp"

void convertFromJson(JsonVariantConst src, wifiCredentials& dst) {
	dst.ssid = src["ssid"].as<String>();
	dst.password = src["password"].as<String>();
}

void convertToJson(wifiCredentials src, JsonVariant variant) {
	variant["ssid"] = src.ssid;
	variant["password"] = src.password;
}