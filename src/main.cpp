#include <Arduino.h>

#include "file_system.module.hpp"
#include "wifi.module.hpp"
#include "server.module.hpp"
#include "tds.module.hpp"
#include "ph_sensor.module.hpp"
#include "updater.module.hpp"

#define FIRMWARE_VERSION "0.0.4"

// Modules
WifiModule *wifi_module = WifiModule::GetInstance();
TDSModule *tds_module = TDSModule::GetInstance();
PHSensorModule *ph_sensor_module = PHSensorModule::GetInstance();
FileSystemModule *fs_module = FileSystemModule::GetInstance();
ServerModule *server_module = ServerModule::GetInstance();
UpdaterModule *updater_module = UpdaterModule::GetInstance();

void setup()
{
  Serial.begin(115200);

  tds_module->onSetup();
  ph_sensor_module->onSetup();
  fs_module->onSetup();
  wifi_module->onSetup();
  updater_module->onSetup(FIRMWARE_VERSION);
  // Server must be latest so modules can register their routes
  // TODO: Move routes registration to server module
  server_module->onSetup();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  ph_sensor_module->getValue();
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
}
