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

	JsonDocument getNetworks();
	void saveNetwork(wifiCredentials);
	void setNetworks(JsonDocument doc);
	void editNetwork(int index, int toIndex, wifiCredentials creds);
	void deleteNetwork(int index);

	protected:
	WifiModule() {}
	~WifiModule() {}

	public:
	static WifiModule *GetInstance();
	void onSetup();
};