#include <Arduino.h>

#include "file_system.module.hpp"
#include "wifi.module.hpp"
#include "server.module.hpp"
#include "tds.module.hpp"
#include "updater.module.hpp"
#include "telegram.module.hpp"

#define FIRMWARE_VERSION "0.0.4"

// Modules
TDSModule *tds_module = TDSModule::GetInstance();

void setup()
{
  Serial.begin(115200);

  tds_module->onSetup();
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

void loop() { }
