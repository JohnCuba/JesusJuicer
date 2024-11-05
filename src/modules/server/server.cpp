#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "modules/wifi/wifi.h"
#include "entities/wifi/credentials.h"
#include "server.h"

ServerModule::ServerModule() :
	server{80}
{};

void ServerModule::log(String message) {
	Serial.println("[SERVER]: " + message);
}

AwsTemplateProcessor ServerModule::connectProcessor(String ssid, String password) {
	return [=](const String& var) {
		if(var == "SSID") {
			return ssid;
		};
		if (var == "PASSWORD") {
			return password;
		};
		return String();
	};
}

void ServerModule::registerApRoutes(WifiModule wifi_module) {
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->redirect("/connect");
	});
};

void ServerModule::onSetup(WifiModule wifi_module) {
	bool isConnected = wifi_module.isConnectedToAp();
	APCredentials hostApCredentials = wifi_module.getHostApCredentials();

	server.on("/connect", HTTP_GET, [=](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/connect.html", String(), false, connectProcessor(hostApCredentials.ssid, hostApCredentials.password));
	});

	server.on("/connect", HTTP_POST, [&wifi_module](AsyncWebServerRequest *request){
		int params = request->params();
		APCredentials hostApCredentials{"", ""};

		for(int i=0; i<params; i++) {
			const AsyncWebParameter* p = request->getParam(i);
			String param = p->value().c_str();

			if (p->name() == "ssid") {
				hostApCredentials.ssid = param;
			}
			if (p->name() == "password") {
				hostApCredentials.password = param;
			}
		}

		wifi_module.setHostApCredentials(hostApCredentials);

		request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: ");
		delay(1000);
		ESP.restart();
	});

	if (!isConnected) {
		registerApRoutes(wifi_module);
	}

	server.begin();
};
