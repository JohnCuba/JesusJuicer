#include <WiFi.h>
#include <Preferences.h>

#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "wifi.module.hpp"
#include "wifi_credentials.hpp"
#include "server.module.hpp"

const long interval = 5000;
const String credsFilePath = "/private/credentials.json";

const String selfApSSID = "aquaphobic";
const String selfApPassword = "yoitsmeman";

WifiModule* WifiModule::pinstance_{nullptr};

WifiModule *WifiModule::GetInstance() {
	if (pinstance_ == nullptr) {
		pinstance_ = new WifiModule();
	}

	return pinstance_;
}

JsonArray putNetwork(JsonArray array, int index, wifiCredentials creds) {
	array[index].set(creds);

	return array;
}

JsonArray moveNetwork(JsonArray array, int index, int to) {
	if (index == to) return array;

	JsonObject obj = array[index];

	array[index].set(array[to]);
	array[to].set(obj);

	return array;
}

wifiCredentials getSelfAPCredentials() {
	Preferences preferences;
  preferences.begin("self-ap", false);

  String ssid = preferences.getString("ssid", selfApSSID); 
  String password = preferences.getString("password", selfApPassword);

	preferences.end();

	return wifiCredentials{ssid, password};
}

void setSelfAPCredentials(wifiCredentials creds) {
	Preferences preferences;
  preferences.begin("self-ap", false);

  preferences.putString("ssid", creds.ssid); 
  preferences.getString("password", creds.password);

	preferences.end();
}

void WifiModule::editNetwork(int index, int toIndex, wifiCredentials creds) {
  JsonDocument doc = getNetworks();
	JsonArray savedCreds = doc.as<JsonArray>();

	if (index >= savedCreds.size()) {
		throw std::invalid_argument("received invalid index");
	}

	putNetwork(savedCreds, index, creds);
	if (index != toIndex) {
		// TODO: clean up shit here
		int resolvedTo = std::max(std::min(String(savedCreds.size()).toInt(), String(toIndex).toInt()), String(0).toInt());
		moveNetwork(savedCreds, index, resolvedTo);
	}

	setNetworks(doc);
}

void WifiModule::saveNetwork(wifiCredentials creds) {
  JsonDocument doc = getNetworks();
	JsonArray savedCreds = doc.as<JsonArray>();

	savedCreds.add(creds);

	setNetworks(doc);
}

void WifiModule::deleteNetwork(int index) {
  JsonDocument doc = getNetworks();
	JsonArray savedCreds = doc.as<JsonArray>();

	savedCreds.remove(index);

	setNetworks(doc);
}

JsonDocument WifiModule::getNetworks() {
	FileSystemModule* fs_module = FileSystemModule::GetInstance();
	String fileContent = fs_module->readFile(credsFilePath);
  JsonDocument doc;
  deserializeJson(doc, fileContent);
	return doc;
}

void WifiModule::setNetworks(JsonDocument doc) {
	FileSystemModule* fs_module = FileSystemModule::GetInstance();
	fs_module->writeFile(credsFilePath, doc);
}

void WifiModule::onSetup() {
	logg.info("start setup");
	logg.info("setup server routes");

	ServerModule* server_module = ServerModule::GetInstance();

	server_module->registerRoute("/api/wifi/ap", HTTP_GET, [=](AsyncWebServerRequest *request) {
		JsonDocument responseBody;
		JsonObject root = responseBody.to<JsonObject>();

		wifiCredentials creds = getSelfAPCredentials();

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

		setSelfAPCredentials(wifiCredentials{ssid, password});

		request->send_P(200, "text/plain", "saved");
	});

	server_module->registerRoute("/api/wifi", HTTP_OPTIONS, [=](AsyncWebServerRequest *request) {
		request->send_P(200, "text/plain", "ok");
	});

	server_module->registerRoute("/api/wifi", HTTP_GET, [=](AsyncWebServerRequest *request) {
		FileSystemModule* fs_module = FileSystemModule::GetInstance();
		String fileContent = fs_module->readFile(credsFilePath);
		AsyncWebServerResponse *response = request->beginResponse(200, "application/json", fileContent);

		request->send(response);
	});

	server_module->registerRoute("/api/wifi", HTTP_POST, [=](AsyncWebServerRequest *request) {
		const String ssid = request->getParam("ssid", true)->value();
		const String password = request->getParam("password", true)->value();

		saveNetwork(wifiCredentials{ ssid, password });

		AsyncWebServerResponse *response = request->beginResponse(201, "text/plain", "Credentials saved");

		request->send(response);
	});

	server_module->registerRoute("/api/wifi", HTTP_PATCH, [=](AsyncWebServerRequest *request) {
		if (!request->hasParam("index", true)) {
			request->send_P(422, "text/plain", "provide index");
		}

		if (!request->hasParam("ssid", true)) {
			request->send_P(422, "text/plain", "provide ssid");
		}

		const int index = request->getParam("index", true)->value().toInt();
		const int to = request->hasParam("to", true) ? request->getParam("to", true)->value().toInt() : index;
		String ssid = request->getParam("ssid", true)->value();
		String password = request->hasParam("password", true) ? request->getParam("password", true)->value() : String();

		try {
			editNetwork(
				index,
				to,
				wifiCredentials{ssid, password}
			);
		} catch (const std::invalid_argument& e) {
			request->send_P(422, "text/plain", e.what());
		}

		AsyncWebServerResponse *response = request->beginResponse(200);

		request->send(response);
	});

	server_module->registerRoute("/api/wifi", HTTP_DELETE, [=](AsyncWebServerRequest *request) {
		if (!request->hasParam("index")) {
			request->send_P(422, "text/plain", "provide index");
		}

		const int index = request->getParam("index")->value().toInt();

		deleteNetwork(index);

		AsyncWebServerResponse *response = request->beginResponse(200);

		request->send(response);
	});

	// TODO: /api/wifi/self get, put

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

	wifiCredentials creds = getSelfAPCredentials();
	WiFi.softAP(creds.ssid, creds.password);

	logg.info("AP is created: " + WiFi.softAPSSID());
	logg.info("IP address: " + WiFi.softAPIP().toString());
};
