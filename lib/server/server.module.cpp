#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include "wifi.module.hpp"
#include "server.module.hpp"
#include "file_system.module.hpp"
#include "tds.module.hpp"
#include "wifi_credentials.hpp"
#include "./resolver/resolve.layout.hpp"
#include "./controller/home.controller.hpp"

ServerModule* ServerModule::pinstance_{nullptr};

ServerModule *ServerModule::GetInstance() {
	if (pinstance_ == nullptr) {
		pinstance_ = new ServerModule();
	}

	return pinstance_;
}

ServerModule::ServerModule() :
	server{80}
{};

String resolveWifiListEntity(String markupTemplate, String title, String target) {
	String markup = String(markupTemplate);

	markup.replace("%TITLE%", title);
	markup.replace("%ID%", target);
	markup.replace("%TARGET%", target);

	return markup;
}

String resolveSavedWifiList() {
	FileSystemModule *fs_module = FileSystemModule::GetInstance();
	WifiModule *wifi_module = WifiModule::GetInstance();

	String listMarkup = fs_module->readFile("/public/wifi/list.html");
	String listEntityTemplate = fs_module->readFile("/public/wifi/list-entity.html");
	JsonDocument credsDoc = wifi_module->getNetworks();
	JsonArray savedCreds = credsDoc.as<JsonArray>();

	String listEntityesMarkup = String();

	for(int i=0; i<savedCreds.size(); i++) {
		wifiCredentials creds = savedCreds[i].as<wifiCredentials>();
		listEntityesMarkup = listEntityesMarkup + resolveWifiListEntity(listEntityTemplate, creds.ssid, String(i));
	};

	listEntityesMarkup = listEntityesMarkup + resolveWifiListEntity(listEntityTemplate, String("Save new network"), String("new"));

	listMarkup.replace("%NETWORKS_LIST%", listEntityesMarkup);

	return listMarkup;
};

String resolveWifiEditor(String target) {
	FileSystemModule *fs_module = FileSystemModule::GetInstance();
	String editorMarkup = fs_module->readFile("/public/wifi/editor.html");
	String title;
	String action = "Save";

	if (target == "new") {
		title = "Save new wifi";
		editorMarkup.replace("%SSID%", "");
		editorMarkup.replace("%PASSWORD%", "");
	} else if (target.toInt() >= 0) {
		WifiModule *wifi_module = WifiModule::GetInstance();
		JsonDocument credsDoc = wifi_module->getNetworks();
		JsonArray savedCreds = credsDoc.as<JsonArray>();
		wifiCredentials creds = savedCreds[target.toInt()].as<wifiCredentials>();

		title = "Edit " + creds.ssid;
		editorMarkup.replace("%SSID%", creds.ssid);
		editorMarkup.replace("%PASSWORD%", creds.password);
	}

	editorMarkup.replace("%TARGET%", target);
	editorMarkup.replace("%ACTION_TITLE%", title);
	editorMarkup.replace("%ACTION%", action);

	return editorMarkup;
}

AsyncWebServerResponse* wifiController(AsyncWebServerRequest* request) {
	String content = String();
	content = content + resolveSavedWifiList();
	content = content + "<div id=\"wifi-editor\" hx-boost=\"true\"></div>";

	if (request->hasHeader("hx-request")) {
		return request->beginResponse(200, "text/html", content);
	}

	String page = resolveLayout(resolveLayoutArgs{
		page: "Wifi",
		content: content,
	});

	return request->beginResponse(200, "text/html", page);
}

AsyncWebServerResponse* wifiEditorController(AsyncWebServerRequest* request) {
	if(!request->hasParam("target")) {
		return request->beginResponse(400, "text/html", String("Provide target to edit"));
	}
	AsyncWebParameter* targetRaw = request->getParam("target");

	String content;

	content = content + resolveWifiEditor(targetRaw->value());

	if (request->hasHeader("hx-request")) {
		return request->beginResponse(200, "text/html", content);
	}

	content = resolveSavedWifiList() + "<div id=\"wifi-editor\" hx-boost=\"true\">" + content + "</div>";

	String page = resolveLayout(resolveLayoutArgs{
		page: "Wifi",
		content: content,
	});

	return request->beginResponse(200, "text/html", page);
}

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
		content = content + resolveSavedWifiList();
	} else if (targetRaw.toInt() >= 0) {
		// TODO: Implement network editing
	}

	if (request->hasHeader("hx-request")) {
		return request->beginResponse(200, "text/html", content);
	}

	request->redirect("/wifi/");
}

void ServerModule::onSetup() {
	server.serveStatic("/data/public/shared/", LittleFS, "/public/shared/");

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = homeController(request);
		request->send(response);
	});

	server.on("/wifi/", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiController(request);
		request->send(response);
	});

	server.on("/wifi/edit", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiEditorController(request);
		request->send(response);
	});

	server.on("/wifi/edit", HTTP_POST, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = wifiEditController(request);
		request->send(response);
	});

	server.on("/api/tds", HTTP_GET, [=](AsyncWebServerRequest *request) {
		TDSModule* tdsModule = TDSModule::GetInstance();

		request->send(200, "text/plain", String(tdsModule->getValue()));
	});

	server.begin();
};
