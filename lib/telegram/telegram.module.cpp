#include "telegram.module.hpp"

#include "api_config.hpp"
#include "logger.h"
#include "server.module.hpp"
#include "storage.module.hpp"
#include "wifi.module.hpp"

#define BOT_TOKEN "7646562962:AAGCvoHmCU3sNAY1MLvYjV9UEhkPn042Itw"
#define STORE_NAMESPACE "telegram"

const char TelegramModule::loggTag_[9] = "TELEGRAM";
const char tg_store_key[3] = "tg";

UniversalTelegramBot TelegramModule::bot_{BOT_TOKEN, wifiSecureClient};

std::string TelegramModule::getChatId() {
  std::string chat_id =
      StorageModule::getString(tg_store_key, "chat_id", "").c_str();

  return chat_id;
}
void TelegramModule::setChatId(std::string value) {
  StorageModule::setString(tg_store_key, "chat_id", value.c_str());
}

void TelegramModule::registerServerRoutes() {
  ESP_LOGD(TelegramModule::loggTag_, "setup server routes");

  ServerModule::registerRoute(
      "/api/telegram/chat_id", HTTP_GET, [=](AsyncWebServerRequest *request) {
        request->send(200, RES_TYPE_TEXT, TelegramModule::getChatId().c_str());
      });

  ServerModule::registerRoute(
      "/api/telegram/chat_id", HTTP_POST, [=](AsyncWebServerRequest *request) {
        if (!request->hasParam("chat_id", true)) {
          return request->send(422, RES_TYPE_TEXT, "provide chat_id");
        }

        std::string chat_id =
            request->getParam("chat_id", true)->value().c_str();
        TelegramModule::setChatId(chat_id);

        request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
      });

  ServerModule::registerRoute("/api/telegram/ping", HTTP_POST,
                              [=](AsyncWebServerRequest *request) {
                                TelegramModule::sendMessage("ping");
                                request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
                              });
}

void TelegramModule::onSetup() {
  ESP_LOGD(TelegramModule::loggTag_, "start setup");

  wifiSecureClient.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  TelegramModule::registerServerRoutes();

  ESP_LOGD(TelegramModule::loggTag_, "end setup");
}

void TelegramModule::sendMessage(std::string message) {
  bot_.sendSimpleMessage(TelegramModule::getChatId().c_str(), message.c_str(),
                         "");
}