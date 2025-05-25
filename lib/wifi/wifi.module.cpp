#include "wifi.module.hpp"

#include <WiFi.h>

#include <string>

#include "ArduinoJson.h"
#include "api_config.hpp"
#include "server.module.hpp"
#include "storage.module.hpp"

const wifi_credentials defaultSelfAP = wifi_credentials{
  ssid : "juicer",
  password : "yoitsmeman",
};

const char ap_store_key[8] = "wifi/ap";
const char sta_store_key[9] = "wifi/sta";

const char WifiModule::loggTag_[5] = "WIFI";

wifi_credentials WifiModule::getCredentials(
    const char *space, const wifi_credentials &defaultCreds = {"", ""}) {
  std::string ssid =
      StorageModule::getString(space, "ssid", defaultCreds.ssid.c_str())
          .c_str();
  std::string password =
      StorageModule::getString(space, "password", defaultCreds.password.c_str())
          .c_str();

  return {ssid, password};
}

void WifiModule::setCredentials(const char *space,
                                const wifi_credentials &creds) {
  StorageModule::setString(space, "ssid", creds.ssid.c_str());
  StorageModule::setString(space, "password", creds.password.c_str());
}

void WifiModule::deleteCredentials(const char *space) {
  StorageModule::deleteKey(space, "ssid");
  StorageModule::deleteKey(space, "password");
}

void WifiModule::registerServerRoutes() {
  Logg::debug(WifiModule::loggTag_, "setup server routes");

  ServerModule::registerRoute(
      "/api/wifi/state", HTTP_GET, [=](AsyncWebServerRequest *request) {
        JsonDocument responseBody;
        JsonObject root = responseBody.to<JsonObject>();

        wifi_mode_t mode = WiFi.getMode();

        root["mode"].set(mode);
        root["ip"].set(mode == WIFI_MODE_STA ? WiFi.localIP()
                                             : WiFi.softAPIP());
        root["rssi"].set(WiFi.RSSI());

        AsyncWebServerResponse *response = request->beginResponse(
            200, RES_TYPE_JSON, responseBody.as<std::string>().c_str());
        request->send(response);
      });

  ServerModule::registerRoute(
      "/api/wifi/ap", HTTP_GET, [=](AsyncWebServerRequest *request) {
        JsonDocument responseBody;
        JsonObject root = responseBody.to<JsonObject>();

        wifi_credentials creds = getCredentials(ap_store_key, defaultSelfAP);

        root["ssid"].set(creds.ssid);
        root["password"].set(creds.password);

        AsyncWebServerResponse *response = request->beginResponse(
            200, RES_TYPE_JSON, responseBody.as<std::string>().c_str());
        request->send(response);
      });

  ServerModule::registerRoute(
      "/api/wifi/ap", HTTP_PATCH, [=](AsyncWebServerRequest *request) {
        if (!request->hasParam("ssid", true)) {
          return request->send(422, RES_TYPE_TEXT, "provide ssid");
        }

        std::string ssid = request->getParam("ssid", true)->value().c_str();
        std::string password =
            request->hasParam("password", true)
                ? request->getParam("password", true)->value().c_str()
                : "";

        JsonDocument responseBody;

        setCredentials(ap_store_key, wifi_credentials{ssid, password});

        request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
      });

  ServerModule::registerRoute(
      "/api/wifi/network", HTTP_GET, [=](AsyncWebServerRequest *request) {
        JsonDocument responseBody;
        JsonObject root = responseBody.to<JsonObject>();

        wifi_credentials creds = getCredentials(sta_store_key);

        root["ssid"].set(creds.ssid);
        root["password"].set(creds.password);

        AsyncWebServerResponse *response = request->beginResponse(
            200, RES_TYPE_JSON, responseBody.as<std::string>().c_str());
        request->send(response);
      });

  ServerModule::registerRoute(
      "/api/wifi/network", HTTP_PATCH, [=](AsyncWebServerRequest *request) {
        if (!request->hasParam("ssid", true)) {
          return request->send(422, RES_TYPE_TEXT, "provide ssid");
        }

        std::string ssid = request->getParam("ssid", true)->value().c_str();
        std::string password =
            request->hasParam("password", true)
                ? request->getParam("password", true)->value().c_str()
                : "";

        JsonDocument responseBody;

        setCredentials(sta_store_key, wifi_credentials{ssid, password});

        request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
      });

  ServerModule::registerRoute("/api/wifi/network", HTTP_DELETE,
                              [=](AsyncWebServerRequest *request) {
                                deleteCredentials(sta_store_key);

                                request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
                              });
}

void WifiModule::onSetup() {
  Logg::debug(WifiModule::loggTag_, "start setup");

  WiFi.persistent(false);

  registerServerRoutes();

  Logg::debug(WifiModule::loggTag_, "setup connection mode");

  bool connected = connectToAP();

  if (!connected) {
    Logg::warn(WifiModule::loggTag_, "connection filed, switch to AP mode");
    createAP();
  }

  Logg::debug(WifiModule::loggTag_, "end setup");
}

bool WifiModule::connectToAP() {
  wifi_credentials creds = getCredentials(sta_store_key);

  if (creds.ssid.empty()) {
    Logg::warn(WifiModule::loggTag_, "no saved networks finded");
    return false;
  };

  WiFi.mode(WIFI_MODE_STA);

  // TODO: gibberish in terminal on params from storage
  Logg::info(WifiModule::loggTag_, "trying connecting to: %s",
             creds.ssid.c_str());

  WiFi.begin(creds.ssid.c_str(), creds.password.c_str());

  unsigned long currentMillis = millis();
  unsigned long const previousMillis = currentMillis;

  const long interval = 5000;

  while (WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      Logg::error(WifiModule::loggTag_, "failed connecting to: %s", creds.ssid);
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Logg::info(WifiModule::loggTag_, "connected to %s", creds.ssid.c_str());
    return true;
  }

  return false;
};

void WifiModule::createAP() {
  WiFi.disconnect(false, true);
  Logg::info(WifiModule::loggTag_, "set mode: %d", WiFi.mode(WIFI_MODE_AP));

  wifi_credentials creds = getCredentials(ap_store_key, defaultSelfAP);
  Logg::info(WifiModule::loggTag_, "status: %d", WiFi.status());

  Logg::info(WifiModule::loggTag_, "init softAP: %d",
             WiFi.softAP(creds.ssid.c_str(), creds.password.c_str()));

  Logg::info(WifiModule::loggTag_, "AP is created: %s",
             WiFi.softAPSSID().c_str());
  Logg::info(WifiModule::loggTag_, "IP address: %s",
             WiFi.softAPIP().toString());
};
