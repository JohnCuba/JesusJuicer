#include <Arduino.h>

#include "file_system.module.hpp"
#include "wifi.module.hpp"
#include "server.module.hpp"
#include "tds.module.hpp"

// Modules
WifiModule* wifi_module = WifiModule::GetInstance();
TDSModule* tds_module = TDSModule::GetInstance();
FileSystemModule* fs_module = FileSystemModule::GetInstance();
ServerModule* server_module = ServerModule::GetInstance();

void setup() {
  Serial.begin(115200);

  tds_module->onSetup();
  fs_module->onSetup();
  wifi_module->onSetup();
  server_module->onSetup();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() { }
