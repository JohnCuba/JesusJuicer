#include "saved_wifi_list.resolver.hpp"

String savedWifiListResolver() {
	FileSystemModule *fs_module = FileSystemModule::GetInstance();
	WifiModule *wifi_module = WifiModule::GetInstance();

	String listMarkup = fs_module->readFile("/public/wifi/list.html");
	String listEntityTemplate = fs_module->readFile("/public/wifi/list-entity.html");
	JsonDocument credsDoc = wifi_module->getNetworks();
	JsonArray savedCreds = credsDoc.as<JsonArray>();

	String listEntityesMarkup = String();

	for(int i=0; i<savedCreds.size(); i++) {
		wifiCredentials creds = savedCreds[i].as<wifiCredentials>();
		listEntityesMarkup = listEntityesMarkup + wifiListEntityResolver(listEntityTemplate, creds.ssid, String(i));
	};

	listEntityesMarkup = listEntityesMarkup + wifiListEntityResolver(listEntityTemplate, String("Save new network"), String("new"));

	listMarkup.replace("%NETWORKS_LIST%", listEntityesMarkup);

	return listMarkup;
};