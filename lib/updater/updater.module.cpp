#include "updater.module.hpp"

#include <Update.h>

#include "api_config.hpp"
#include "logger.h"
#include "server.module.hpp"

const char UpdaterModule::loggTag_[8] = "UPDATER";

ArRequestHandlerFunction UpdaterModule::updateRequestHandler() {
  return [=](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(
        200, RES_TYPE_TEXT, (Update.hasError()) ? RES_BODY_BAD : RES_BODY_OK);
    response->addHeader("Connection", "close");
    request->send(response);
    delay(100);
    ESP.restart();
  };
};

ArUploadHandlerFunction UpdaterModule::updateUploadHandler(int command) {
  return [=](AsyncWebServerRequest *request, const String &filename,
             size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
      ESP_LOGI(UpdaterModule::loggTag_, "Upload Start: [%s]", filename);
    }

    if (len) {
      if (!Update.begin(UPDATE_SIZE_UNKNOWN, command) && !Update.isRunning()) {
        ESP_LOGE(UpdaterModule::loggTag_, "%s", Update.errorString());
      };

      if (Update.write(data, len) == len) {
        // TODO: make line logger
        Serial.print(".");
      } else {
        ESP_LOGE(UpdaterModule::loggTag_, "%s", Update.errorString());
        Update.abort();
        request->send(500, RES_TYPE_TEXT, Update.errorString());
      }
    }

    if (final) {
      Serial.print("\n");
      if (Update.end(true)) {
        ESP_LOGI(UpdaterModule::loggTag_, "Update success, rebooting...");
      } else {
        Update.printError(Serial);
      }

      ESP_LOGI(UpdaterModule::loggTag_, "Upload Complete: [%s],size: [%n]",
               filename, index + len);
    }
  };
}

void UpdaterModule::registerServerRoutes(const char *fwVersion) {
  ESP_LOGD(UpdaterModule::loggTag_, "updater server routes");

  ServerModule::registerRoute("/api/update/fw", HTTP_GET,
                              [=](AsyncWebServerRequest *request) {
                                request->send(200, RES_TYPE_TEXT, fwVersion);
                              });

  ServerModule::registerRoute("/api/update/fw", HTTP_POST,
                              updateRequestHandler(),
                              updateUploadHandler(U_FLASH));

  ServerModule::registerRoute("/api/update/fs", HTTP_POST,
                              updateRequestHandler(),
                              updateUploadHandler(U_SPIFFS));
}

void UpdaterModule::onSetup(const char *fwVersion) {
  ESP_LOGD(UpdaterModule::loggTag_, "start setup");

  registerServerRoutes(fwVersion);

  ESP_LOGD(UpdaterModule::loggTag_, "end setup");
}