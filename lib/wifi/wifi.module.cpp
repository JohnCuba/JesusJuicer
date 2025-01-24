#include <WiFi.h>
#include "ArduinoJson.h"
#include "wifi.module.hpp"
#include "wifi_credentials.hpp"
#include "server.module.hpp"
#include "./controller/wifi.controller.hpp"
#include "./controller/wifi_editor.controller.hpp"
#include "./controller/wifi_edit.controller.hpp"

const long interval = 5000;
const String credsFilePath = "/private/credentials.json";

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
	logg.info("setup server routes");

	ServerModule* server_module = ServerModule::GetInstance();

	server_module->registerRoute("/wifi/", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiController(request);
		request->send(response);
	});

	server_module->registerRoute("/wifi/edit", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiEditorController(request);
		request->send(response);
	});

	server_module->registerRoute("/wifi/edit", HTTP_POST, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiEditController(request);
		request->send(response);
	});

	logg.info("setup connection mode");

	bool connected = connectToAP();

	if (!connected) {
		logg.info("create AP");
		createAP();
	}

	logg.info("end setup");
}

bool WifiModule::connectToAP() {
	JsonDocument credsDoc = getNetworks();
	JsonArray savedCreds = credsDoc.as<JsonArray>();
	logg.info("found " + String(savedCreds.size()) + " saved networks");

	if (savedCreds.size() < 1) {
		logg.info("no saved networks finded");
		return false;
	};

	WiFi.mode(WIFI_MODE_STA);

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
			logg.info("connected to " + creds.ssid + " go to " + WiFi.localIP().toString());
			// TODO: Move connected network on 0 index
			return true;
		}
	}

	logg.info("Saved networks: " + String(savedCreds.size()) + ", but no one there");

	return false;
};

void WifiModule::createAP() {
	WiFi.mode(WIFI_MODE_AP);
	WiFi.softAP("aquaphobic", "yoitsmeman");

	logg.info("AP is created: " + WiFi.softAPSSID());
	logg.info("IP address: " + WiFi.softAPIP().toString());
};
