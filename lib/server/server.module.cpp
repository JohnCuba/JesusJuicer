#include "server.module.hpp"

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include "LittleFS.h"
#include "api_config.hpp"
#include "logger.h"
#include "tds.module.hpp"

const char ServerModule::loggTag_[7] = "SERVER";

AsyncWebServer ServerModule::server{80};

void ServerModule::registerRoute(const char *uri,
                                 WebRequestMethodComposite method,
                                 ArRequestHandlerFunction onRequest) {
  server.on(uri, method, onRequest);
};

void ServerModule::registerRoute(const char *uri,
                                 WebRequestMethodComposite method,
                                 ArRequestHandlerFunction onRequest,
                                 ArUploadHandlerFunction onUpload) {
  server.on(uri, method, onRequest, onUpload);
};

void ServerModule::onSetup() {
  ESP_LOGD(ServerModule::loggTag_, "start setup");
  MDNS.begin("juicer");

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader(
      "Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
  DefaultHeaders::Instance().addHeader(
      "Access-Control-Allow-Headers",
      "Content-Type, Content-Disposition, Authorization, X-Custom-Header");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Private-Network",
                                       "true");

  server.serveStatic("/", LittleFS, "/public/");

  server.on("/*", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response =
        request->beginResponse(LittleFS, "/public/index.html.gz", "text/html");
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  server.on("/api/*", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
    request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
  });

  server.begin();
};
