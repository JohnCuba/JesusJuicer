#include "../../entities/wifi/credentials.h"
#include <Preferences.h>

#pragma once

class WifiModule {
	private:
	char* SSID_KEY;
	char* PASSWORD_KEY;
	APCredentials localApCreds;
	Preferences preferences;
	bool isLocalApActive;
	void log(String);
	void resetHostApCredentials();
	void createAP();
	bool connectToAP(APCredentials);

	public:
	WifiModule();
	void onSetup();
	void onLoop();
	APCredentials getHostApCredentials();
	void setHostApCredentials(APCredentials creds);
	void setHostApCredentials(String ssid, String password);
	bool isConnectedToAp();
};