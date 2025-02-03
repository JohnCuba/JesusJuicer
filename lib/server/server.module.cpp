#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include "server.module.hpp"
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
{
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Private-Network", "true");
};

void ServerModule::registerRoute(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {
	server.on(uri, method, onRequest);
};

void ServerModule::onSetup() {
	server.serveStatic("/", LittleFS, "/public/");

	server.on("/*", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/public/index.html.gz", "text/html");
		response->addHeader("Content-Encoding", "gzip");
		request->send(response);
	});

	server.on("/api/tds", HTTP_GET, [=](AsyncWebServerRequest *request) {
		TDSModule* tdsModule = TDSModule::GetInstance();

		request->send(200, "text/plain", String(tdsModule->getValue()));
	});

	server.begin();
};
