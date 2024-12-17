#include <WiFi.h>
#include "ArduinoJson.h"
#include "wifi.module.hpp"
#include "wifi_credentials.hpp"

const long interval = 5000;
const String credsFilePath = "/private/credentials.json";

void convertFromJson(JsonVariantConst src, wifiCredentials& dst) {
	dst.ssid = src["ssid"].as<String>();
	dst.password = src["password"].as<String>();
}

void convertToJson(wifiCredentials src, JsonVariant variant) {
	variant["ssid"] = src.ssid;
	variant["password"] = src.password;
}

WifiModule* WifiModule::pinstance_{nullptr};

WifiModule *WifiModule::GetInstance() {
	if (pinstance_ == nullptr) {
		pinstance_ = new WifiModule();
	}

	return pinstance_;
}

void WifiModule::saveNetwork(wifiCredentials creds) {
	FileSystemModule* fs_module = FileSystemModule::GetInstance();
	String fileContent = fs_module->readFile(credsFilePath);
  JsonDocument doc;
  deserializeJson(doc, fileContent);
	JsonArray savedCreds = doc.as<JsonArray>();
	// TODO: Write network on 0 index
	savedCreds.add(creds);

	fs_module->writeFile(credsFilePath, doc);
}

JsonDocument WifiModule::getNetworks() {
	FileSystemModule* fs_module = FileSystemModule::GetInstance();
	String fileContent = fs_module->readFile(credsFilePath);
  JsonDocument doc;
  deserializeJson(doc, fileContent);
	return doc;
}

void WifiModule::onSetup() {
	logg.info("start setup");

	bool connected = connectToAP();

	if (!connected) {
		logg.info("create AP");
		createAP();
	}

	logg.info("end setup");
}

bool WifiModule::connectToAP() {
	JsonArray savedCreds = getNetworks().as<JsonArray>();
	WiFi.mode(WIFI_MODE_STA);

	logg.info("found " + String(savedCreds.size()) + " saved networks");

	for (int i = 0; i < savedCreds.size(); ++i) {
		wifiCredentials creds = savedCreds[i].as<wifiCredentials>();

		logg.info("trying connecting to: " + creds.ssid);

		WiFi.begin(creds.ssid, creds.password);

		unsigned long currentMillis = millis();
		unsigned long previousMillis = currentMillis;

		while(WiFi.status() != WL_CONNECTED) {
			currentMillis = millis();

			if (currentMillis - previousMillis >= interval) {
				logg.info("failed connecting to: " + creds.ssid);
				break;
			}
		}

		if (WiFi.status() == WL_CONNECTED) {
			// TODO: Move connected network on 0 index
			return true;
		}
	}

	return false;
};

void WifiModule::createAP() {
	WiFi.mode(WIFI_MODE_AP);
	WiFi.softAP("aquaphobic", "yoitsmeman");

	logg.info("AP is created: " + WiFi.softAPSSID());
	logg.info("IP address: " + WiFi.softAPIP().toString());
};
