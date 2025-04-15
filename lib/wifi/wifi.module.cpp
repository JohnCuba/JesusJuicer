#include <string>
#include <WiFi.h>
#include <Preferences.h>

#include "ArduinoJson.h"
#include "wifi.module.hpp"
#include "wifi_credentials.hpp"
#include "server.module.hpp"
#include "api_config.hpp"

const wifiCredentials defaultSelfAP = wifiCredentials{
	ssid : "juicer",
	password : "yoitsmeman",
};

const char selfAPStoreKey[8] = "self-ap";
const char networkAPStoreKey[11] = "network-ap";

const char WifiModule::loggTag_[5] = "WIFI";

wifiCredentials WifiModule::getAPCredentials(const char *key, const wifiCredentials &defaultCreds = {"", ""})
{
	Preferences preferences;
	if (!preferences.begin(key, true))
	{
		Logg::error("Error open preferences key: %s", key);
		return defaultCreds;
	}

	std::string ssid = preferences.getString("ssid", defaultCreds.ssid.c_str()).c_str();
	std::string password = preferences.getString("password", defaultCreds.password.c_str()).c_str();

	preferences.end();

	return {ssid, password};
}

void WifiModule::setAPCredentials(const char *key, const wifiCredentials &creds)
{
	Preferences preferences;
	if (!preferences.begin(key, false))
	{
		Logg::error("Error open preferences key: %s", key);
		return;
	}

	preferences.putString("ssid", creds.ssid.c_str());
	preferences.putString("password", creds.password.c_str());
}

void WifiModule::deleteAPCredentials(const char *key)
{
	Preferences preferences;
	if (!preferences.begin(key, false))
	{
		Logg::error("Error open preferences key: %s", key);
		return;
	}

	if (!preferences.clear())
	{
		Logg::error("Error clear preferences key: %s", key);
		return;
	}

	preferences.end();
}

void WifiModule::registerServerRoutes()
{
	Logg::debug(WifiModule::loggTag_, "setup server routes");

	ServerModule::registerRoute(
			"/api/wifi/state",
			HTTP_GET,
			[=](AsyncWebServerRequest *request)
			{
				JsonDocument responseBody;
				JsonObject root = responseBody.to<JsonObject>();

				wifi_mode_t mode = WiFi.getMode();

				root["mode"].set(mode);
				root["ip"].set(mode == WIFI_MODE_STA ? WiFi.localIP() : WiFi.softAPIP());
				root["rssi"].set(WiFi.RSSI());

				AsyncWebServerResponse *response = request->beginResponse(200, RES_TYPE_JSON, responseBody.as<String>());
				request->send(response);
			});

	ServerModule::registerRoute(
			"/api/wifi/ap",
			HTTP_GET,
			[=](AsyncWebServerRequest *request)
			{
				JsonDocument responseBody;
				JsonObject root = responseBody.to<JsonObject>();

				wifiCredentials creds = getAPCredentials(selfAPStoreKey, defaultSelfAP);

				root["ssid"].set(creds.ssid);
				root["password"].set(creds.password);

				AsyncWebServerResponse *response = request->beginResponse(200, RES_TYPE_JSON, responseBody.as<String>());
				request->send(response);
			});

	ServerModule::registerRoute(
			"/api/wifi/ap",
			HTTP_PATCH,
			[=](AsyncWebServerRequest *request)
			{
				if (!request->hasParam("ssid", true))
				{
					return request->send(422, RES_TYPE_TEXT, "provide ssid");
				}

				std::string ssid = request->getParam("ssid", true)->value().c_str();
				std::string password = request->hasParam("password", true) ? request->getParam("password", true)->value().c_str() : "";

				JsonDocument responseBody;

				setAPCredentials(selfAPStoreKey, wifiCredentials{ssid, password});

				request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
			});

	ServerModule::registerRoute(
			"/api/wifi/network",
			HTTP_GET,
			[=](AsyncWebServerRequest *request)
			{
				JsonDocument responseBody;
				JsonObject root = responseBody.to<JsonObject>();

				wifiCredentials creds = getAPCredentials(networkAPStoreKey);

				root["ssid"].set(creds.ssid);
				root["password"].set(creds.password);

				AsyncWebServerResponse *response = request->beginResponse(200, RES_TYPE_JSON, responseBody.as<String>());
				request->send(response);
			});

	ServerModule::registerRoute(
			"/api/wifi/network",
			HTTP_PATCH,
			[=](AsyncWebServerRequest *request)
			{
				if (!request->hasParam("ssid", true))
				{
					return request->send(422, RES_TYPE_TEXT, "provide ssid");
				}

				std::string ssid = request->getParam("ssid", true)->value().c_str();
				std::string password = request->hasParam("password", true) ? request->getParam("password", true)->value().c_str() : "";

				JsonDocument responseBody;

				setAPCredentials(networkAPStoreKey, wifiCredentials{ssid, password});

				request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
			});

	ServerModule::registerRoute(
			"/api/wifi/network",
			HTTP_DELETE,
			[=](AsyncWebServerRequest *request)
			{
				deleteAPCredentials(networkAPStoreKey);

				request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
			});
}

void WifiModule::onSetup()
{
	Logg::debug(WifiModule::loggTag_, "start setup");

	registerServerRoutes();

	Logg::debug(WifiModule::loggTag_, "setup connection mode");

	bool connected = connectToAP();

	if (!connected)
	{
		Logg::warn(WifiModule::loggTag_, "connection filed, switch to AP mode");
		createAP();
	}

	Logg::debug(WifiModule::loggTag_, "end setup");
}

bool WifiModule::connectToAP()
{
	wifiCredentials creds = getAPCredentials(networkAPStoreKey);

	if (creds.ssid.empty())
	{
		Logg::warn(WifiModule::loggTag_, "no saved networks finded");
		return false;
	};

	WiFi.mode(WIFI_MODE_STA);

	// TODO: gibberish in terminal on params from Preferences
	Logg::info(WifiModule::loggTag_, "trying connecting to: %s", creds.ssid.c_str());

	WiFi.begin(creds.ssid.c_str(), creds.password.c_str());

	unsigned long currentMillis = millis();
	unsigned long const previousMillis = currentMillis;

	const long interval = 5000;

	while (WiFi.status() != WL_CONNECTED)
	{
		currentMillis = millis();

		if (currentMillis - previousMillis >= interval)
		{
			Logg::error(WifiModule::loggTag_, "failed connecting to: %s", creds.ssid);
			break;
		}
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		Logg::info(WifiModule::loggTag_, "connected to %s", creds.ssid.c_str());
		return true;
	}

	return false;
};

void WifiModule::createAP()
{
	WiFi.mode(WIFI_MODE_AP);

	wifiCredentials creds = getAPCredentials(selfAPStoreKey, defaultSelfAP);
	WiFi.softAP(creds.ssid.c_str(), creds.password.c_str());

	Logg::info(WifiModule::loggTag_, "AP is created: %s", WiFi.softAPSSID());
	Logg::info(WifiModule::loggTag_, "IP address: %s", WiFi.softAPIP());
};
