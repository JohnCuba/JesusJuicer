#include "wifi_edit.controller.hpp"
#include "wifi.module.hpp"
#include "../resolver/saved_wifi_list.resolver.hpp"

AsyncWebServerResponse* wifiEditController(AsyncWebServerRequest* request) {
	if(!request->hasParam("target")) {
		return request->beginResponse(400, "text/html", String("Provide target to edit"));
	} else if (!request->hasParam("ssid", true)) {
		return request->beginResponse(400, "text/html", String("Provide ssid to edit"));
	}

	String targetRaw = request->getParam("target")->value();
	String ssid = request->getParam("ssid", true)->value();
	String password = request->getParam("password", true)->value();

	WifiModule *wifi_module = WifiModule::GetInstance();

	String content;

	if (targetRaw == "new") {
		wifi_module->saveNetwork(wifiCredentials{ ssid, password });
		content = content + savedWifiListResolver();
	} else if (targetRaw.toInt() >= 0) {
		// TODO: Implement network editing
	}

	if (request->hasHeader("hx-request")) {
		return request->beginResponse(200, "text/html", content);
	}

	request->redirect("/wifi/");
}