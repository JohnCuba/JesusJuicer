#include "wifi.h"
#include "../../entities/wifi/credentials.h"
#include <WiFi.h>
#include <Preferences.h>

WifiModule::WifiModule() :
	SSID_KEY{"ssid"},
	PASSWORD_KEY{"password"},
	localApCreds{"aquaphobic", "yoitsmeman"},
	isLocalApActive{false}
{};

void WifiModule::onSetup() {
	preferences.begin("wifi_networks", false);
	APCredentials hostApCredentials = getHostApCredentials();

	if (hostApCredentials.isEmpty()) {
		log("Empty credentials");
		createAP();
		return;
	}

	bool isConnected = connectToAP(hostApCredentials);

	if (!isConnected) {
		resetHostApCredentials();
		createAP();
		return;
	}
}

void WifiModule::onLoop() {
	if (!isLocalApActive && !WiFi.isConnected()) {
		createAP();
	}
}

void WifiModule::log(String message) {
	Serial.println("[WIFI]: " + message);
}

bool WifiModule::isConnectedToAp() {
	return WiFi.isConnected();
}

APCredentials WifiModule::getHostApCredentials() {
	APCredentials hostApCredentials{preferences.getString(SSID_KEY, ""), preferences.getString(PASSWORD_KEY, "")};

	return hostApCredentials;
};

void WifiModule::setHostApCredentials(String ssid, String password) {
	preferences.putString(SSID_KEY, ssid);
	preferences.putString(PASSWORD_KEY, password);
}

void WifiModule::setHostApCredentials(APCredentials creds) {
	setHostApCredentials(creds.ssid, creds.password);
}

void WifiModule::resetHostApCredentials() {
	preferences.clear();
}

bool WifiModule::connectToAP(APCredentials credentials) {
	WiFi.mode(WIFI_MODE_STA);

	wl_status_t status = WiFi.begin(credentials.ssid.c_str(), credentials.password.c_str());

	log("Connectiong to: " + credentials.ssid);

	while (WiFi.status() != WL_CONNECTED) {
		delay(200);
		log(".");
	}

	log("Connected to: " + WiFi.SSID());
	log("IP address: " + WiFi.localIP().toString());
	isLocalApActive = false;
	return true;
};

void WifiModule::createAP() {
	WiFi.mode(WIFI_MODE_AP);
	WiFi.softAP(localApCreds.ssid, localApCreds.password);
	isLocalApActive = true;

	log("AP is created: " + WiFi.softAPSSID());
	log("IP address: " + WiFi.softAPIP().toString());
};
