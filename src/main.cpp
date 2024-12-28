#include <Arduino.h>

#include "file_system.module.hpp"
#include "wifi.module.hpp"
#include "server.module.hpp"
#include "tds.module.hpp"

// Modules
WifiModule* wifi_module = WifiModule::GetInstance();
FileSystemModule* fs_module = FileSystemModule::GetInstance();
ServerModule* server_module = ServerModule::GetInstance();
TDSModule* tds_module = TDSModule::GetInstance();

void setup() {
  Serial.begin(115200);

  fs_module->onSetup();
  wifi_module->onSetup();
  server_module->onSetup();
  tds_module->onSetup();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  tds_module->onLoop();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
