#include <Arduino.h>
#include "SPIFFS.h"
#include "modules/wifi/wifi.h"
#include "modules/server/server.h"

// Modules
WifiModule wifi_module = WifiModule();
ServerModule server_module = ServerModule();

void setup() {
  Serial.begin(115200);
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  wifi_module.onSetup();
  server_module.onSetup(wifi_module);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  wifi_module.onLoop();
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
