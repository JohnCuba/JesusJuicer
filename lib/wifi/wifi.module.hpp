#include <WiFiClientSecure.h>

#include "file_system.module.hpp"
#include "logg.hpp"
#include "wifi_credentials.hpp"

#pragma once

static WiFiClientSecure wifiSecureClient;

class WifiModule {
 private:
  static const char loggTag_[5];
  static void createAP();
  static bool connectToAP();
  static void registerServerRoutes();
  static wifiCredentials getCredentials(const char *space,
                                        const wifiCredentials &creds);
  static void setCredentials(const char *space, const wifiCredentials &creds);
  static void deleteCredentials(const char *space);

 public:
  static void onSetup();
};