#include <Arduino.h>

#include "file_system.module.hpp"
#include "server.module.hpp"
#include "storage.module.hpp"
#include "tds.module.hpp"
#include "telegram.module.hpp"
#include "updater.module.hpp"
#include "wifi.module.hpp"

#define FIRMWARE_VERSION "0.0.5"

void setup() {
  Serial.begin(115200);

  StorageModule::setup();
  TDSModule::onSetup();
  FileSystemModule::onSetup();
  WifiModule::onSetup();
  UpdaterModule::onSetup(FIRMWARE_VERSION);
  TelegramModule::onSetup();
  // Server must be latest so modules can register their routes
  // TODO: Move routes registration to server module
  ServerModule::onSetup();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {}
