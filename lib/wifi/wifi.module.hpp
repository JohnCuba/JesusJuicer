#include <WiFiClientSecure.h>

#include <string>

#pragma once

struct wifi_credentials {
  std::string ssid;
  std::string password;
};

static WiFiClientSecure wifiSecureClient;

class WifiModule {
 private:
  static const char loggTag_[5];
  static void createAP();
  static bool connectToAP();
  static void registerServerRoutes();
  static wifi_credentials getCredentials(const char *space,
                                         const wifi_credentials &creds);
  static void setCredentials(const char *space, const wifi_credentials &creds);
  static void deleteCredentials(const char *space);

 public:
  static void onSetup();
};