#include <WiFi.h>
#include <Preferences.h>

#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "wifi.module.hpp"
#include "wifi_credentials.hpp"
#include "server.module.hpp"

const long interval = 5000;
const String credsFilePath = "/private/credentials.json";

const wifiCredentials defaultSelfAP = wifiCredentials{
	ssid: "aquaphobic",
	password: "yoitsmeman",
};

const char selfAPStoreKey[8] = "self-ap";
const char networkAPStoreKey[11] = "network-ap";

WifiModule* WifiModule::pinstance_{nullptr};

WifiModule *WifiModule::GetInstance() {
	if (pinstance_ == nullptr) {
		pinstance_ = new WifiModule();
	}

	return pinstance_;
}

wifiCredentials getAPCredentials(const char *key, wifiCredentials defaultCreds = wifiCredentials{ssid: "", password: ""}) {
	Preferences preferences;
  preferences.begin(key, true);

  String ssid = preferences.getString("ssid", defaultCreds.ssid); 
  String password = preferences.getString("password", defaultCreds.password);

	preferences.end();

	return wifiCredentials{ssid, password};
}

void setAPCredentials(const char *key, wifiCredentials creds) {
	Preferences preferences;
  preferences.begin(key, false);

  preferences.putString("ssid", creds.ssid); 
  preferences.putString("password", creds.password);

	preferences.end();
}

void deleteAPCredentials(const char *key) {
	Preferences preferences;
  preferences.begin(key, false);

	preferences.clear();

	preferences.end();
}

void WifiModule::registerServerRoutes() {
	logg.info("setup server routes");

	ServerModule* server_module = ServerModule::GetInstance();

	server_module->registerRoute("/api/wifi/state", HTTP_GET, [=](AsyncWebServerRequest *request) {
		JsonDocument responseBody;
		JsonObject root = responseBody.to<JsonObject>();

		String mode = String(WiFi.getMode());
		
		root["mode"].set(mode);
		root["ip"].set(mode == "1" ? WiFi.localIP().toString() : WiFi.softAPIP().toString());
		root["rssi"].set(WiFi.RSSI());

		AsyncWebServerResponse *response = request->beginResponse(200, "application/json", responseBody.as<String>());
		request->send(response);
	});

	server_module->registerRoute("/api/wifi/ap", HTTP_GET, [=](AsyncWebServerRequest *request) {
		JsonDocument responseBody;
		JsonObject root = responseBody.to<JsonObject>();

		wifiCredentials creds = getAPCredentials(selfAPStoreKey, defaultSelfAP);

		root["ssid"].set(creds.ssid);
		root["password"].set(creds.password);

		AsyncWebServerResponse *response = request->beginResponse(200, "application/json", responseBody.as<String>());
		request->send(response);
	});

	server_module->registerRoute("/api/wifi/ap", HTTP_PATCH, [=](AsyncWebServerRequest *request) {
		if (!request->hasParam("ssid", true)) {
			return request->send_P(422, "text/plain", "provide ssid");
		}

		String ssid = request->getParam("ssid", true)->value();
		String password = request->hasParam("password", true) ? request->getParam("password", true)->value() : String();

		JsonDocument responseBody;

		setAPCredentials(selfAPStoreKey, wifiCredentials{ssid, password});

		request->send_P(200, "text/plain", "saved");
	});

	server_module->registerRoute("/api/wifi/network", HTTP_GET, [=](AsyncWebServerRequest *request) {
		JsonDocument responseBody;
		JsonObject root = responseBody.to<JsonObject>();

		wifiCredentials creds = getAPCredentials(networkAPStoreKey);

		root["ssid"].set(creds.ssid);
		root["password"].set(creds.password);

		AsyncWebServerResponse *response = request->beginResponse(200, "application/json", responseBody.as<String>());
		request->send(response);
	});

	server_module->registerRoute("/api/wifi/network", HTTP_PATCH, [=](AsyncWebServerRequest *request) {
		if (!request->hasParam("ssid", true)) {
			return request->send_P(422, "text/plain", "provide ssid");
		}

		String ssid = request->getParam("ssid", true)->value();
		String password = request->hasParam("password", true) ? request->getParam("password", true)->value() : String();
		logg.info(ssid + " " + password);

		JsonDocument responseBody;

		setAPCredentials(networkAPStoreKey, wifiCredentials{ssid, password});

		request->send_P(200, "text/plain", "saved");
	});

	server_module->registerRoute("/api/wifi/network", HTTP_DELETE, [=](AsyncWebServerRequest *request) {
		deleteAPCredentials(networkAPStoreKey);

		request->send_P(200, "text/plain", "deleted");
	});

	server_module->registerRoute("/api/wifi", HTTP_OPTIONS, [=](AsyncWebServerRequest *request) {
		request->send_P(200, "text/plain", "ok");
	});
}

void WifiModule::onSetup() {
	logg.info("start setup");

	registerServerRoutes();

	logg.info("setup connection mode");

	bool connected = connectToAP();

	if (!connected) {
		logg.info("create AP");
		createAP();
	}

	logg.info("end setup");
}

bool WifiModule::connectToAP() {
	wifiCredentials creds = getAPCredentials(networkAPStoreKey);

	if (creds.ssid.isEmpty()) {
		logg.info("no saved networks finded");
		return false;
	};

	WiFi.mode(WIFI_MODE_STA);

	logg.info("trying connecting to: " + creds.ssid + " " + creds.password);

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
		logg.info("connected to " + creds.ssid + " go to " + WiFi.localIP().toString());
		return true;
	}

	return false;
};

void WifiModule::createAP() {
	WiFi.mode(WIFI_MODE_AP);

	wifiCredentials creds = getAPCredentials(selfAPStoreKey, defaultSelfAP);
	WiFi.softAP(creds.ssid, creds.password);

	logg.info("AP is created: " + WiFi.softAPSSID());
	logg.info("IP address: " + WiFi.softAPIP().toString());
};
