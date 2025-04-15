#include <WiFiClientSecure.h>
#include "logg.hpp"
#include "file_system.module.hpp"
#include "wifi_credentials.hpp"

#pragma once

static WiFiClientSecure wifiSecureClient;

class WifiModule
{
private:
	static const char loggTag_[5];
	static void createAP();
	static bool connectToAP();
	static void registerServerRoutes();
	static wifiCredentials getAPCredentials(const char *key, const wifiCredentials &);
	static void setAPCredentials(const char *key, const wifiCredentials &creds);
	static void deleteAPCredentials(const char *key);

public:
	static void onSetup();
};