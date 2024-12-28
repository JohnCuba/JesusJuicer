#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include "wifi.module.hpp"
#include "server.module.hpp"
#include "file_system.module.hpp"
#include "tds.module.hpp"

ServerModule* ServerModule::pinstance_{nullptr};

ServerModule *ServerModule::GetInstance() {
	if (pinstance_ == nullptr) {
		pinstance_ = new ServerModule();
	}

	return pinstance_;
}

ServerModule::ServerModule() :
	server{80}
{};

AwsTemplateProcessor connectProcessor() {
	return [=](const String& var) {
		return String();
	};
}

void ServerModule::onSetup() {
	server.serveStatic("/data/", LittleFS, "/");

	server.on("/connect", HTTP_GET, [=](AsyncWebServerRequest *request){
		request->send(LittleFS, "/connect.html", String(), false, connectProcessor());
	});

	server.on("/connect", HTTP_POST, [=](AsyncWebServerRequest *request){
		int params = request->params();
		wifiCredentials creds;

		for(int i=0; i<params; i++) {
			const AsyncWebParameter* p = request->getParam(i);
			String value = p->value().c_str();

			if (p->name() == "ssid") {
				creds.ssid = value;
			}

			if (p->name() == "password") {
				creds.password = value;
			}
		}

		request->send(LittleFS, "/connecting.html", String(), false, connectProcessor());

  	WifiModule* wifi_module = WifiModule::GetInstance();

		wifi_module->saveNetwork(creds);

		logg.info("Network " + creds.ssid + " saved");

		delay(1000);
		ESP.restart();
	});

	server.on("/api/tds", HTTP_GET, [=](AsyncWebServerRequest *request) {
		TDSModule* tdsModule = TDSModule::GetInstance();

		request->send(200, "text/plain", String(tdsModule->getValue()));
	});

	server.begin();
};
