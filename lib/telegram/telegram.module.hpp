#include <UniversalTelegramBot.h>

#pragma once

class TelegramModule {
 private:
  static const char loggTag_[9];
  static UniversalTelegramBot bot_;
  static std::string getChatId();
  static void setChatId(std::string value);
  static void registerServerRoutes();

 public:
  static void onSetup();

  static void sendMessage(std::string message);
};
