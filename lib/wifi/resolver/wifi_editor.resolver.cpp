#include "wifi_editor.resolver.hpp"
#include "file_system.module.hpp"
#include "wifi.module.hpp"
#include "wifi_credentials.hpp"

String wifiEditorResolver(String target) {
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