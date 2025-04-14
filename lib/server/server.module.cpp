#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "LittleFS.h"
#include "server.module.hpp"
#include "tds.module.hpp"
#include "api_config.hpp"

const char ServerModule::loggTag_[7] = "SERVER";

ServerModule *ServerModule::instance_{nullptr};

ServerModule *ServerModule::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new ServerModule();
	}

	return instance_;
}

ServerModule::ServerModule() : server{80}
{
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Content-Disposition, Authorization, X-Custom-Header");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Private-Network", "true");
};

void ServerModule::registerRoute(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest)
{
	server.on(uri, method, onRequest);
};

void ServerModule::registerRoute(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload)
{
	server.on(uri, method, onRequest, onUpload);
};

void ServerModule::onSetup()
{
	MDNS.begin("juicer");

	server.serveStatic("/", LittleFS, "/public/");

	server.on("/*", HTTP_GET, [](AsyncWebServerRequest *request)
						{
		AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/public/index.html.gz", "text/html");
		response->addHeader("Content-Encoding", "gzip");
		request->send(response); });

	server.on("/api/*", HTTP_OPTIONS, [](AsyncWebServerRequest *request)
						{ request->send(200, RES_TYPE_TEXT, RES_BODY_OK); });

	server.begin();
};
