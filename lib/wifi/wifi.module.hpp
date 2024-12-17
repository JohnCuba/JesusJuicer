#include "logg.hpp"
#include <ArduinoJson.h>
#include "file_system.module.hpp"
#include "wifi_credentials.hpp"

#pragma once

class WifiModule {
	private:
	static WifiModule* pinstance_;
	Logg logg = Logg("WIFI");

	void createAP();
	bool connectToAP();

	protected:
	WifiModule() {}
	~WifiModule() {}

	public:
	static WifiModule *GetInstance();
	void onSetup();
	void saveNetwork(wifiCredentials);
	JsonDocument getNetworks();
};