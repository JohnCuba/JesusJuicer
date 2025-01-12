#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include "server.module.hpp"
#include "tds.module.hpp"
#include "./controller/home.controller.hpp"
#include "./controller/wifi.controller.hpp"
#include "./controller/wifi_edit.controller.hpp"
#include "./controller/wifi_editor.controller.hpp"

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

void ServerModule::onSetup() {
	server.serveStatic("/data/public/shared/", LittleFS, "/public/shared/");

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = homeController(request);
		request->send(response);
	});

	server.on("/wifi/", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiController(request);
		request->send(response);
	});

	server.on("/wifi/edit", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiEditorController(request);
		request->send(response);
	});

	server.on("/wifi/edit", HTTP_POST, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiEditController(request);
		request->send(response);
	});

	server.on("/api/tds", HTTP_GET, [=](AsyncWebServerRequest *request) {
		TDSModule* tdsModule = TDSModule::GetInstance();

		request->send(200, "text/plain", String(tdsModule->getValue()));
	});

	server.begin();
};
