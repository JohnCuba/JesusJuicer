#include "wifi.controller.hpp"
#include "../resolver/saved_wifi_list.resolver.hpp"
#include "../resolver/layout.resolver.hpp"

AsyncWebServerResponse* wifiController(AsyncWebServerRequest* request) {
	String content = String();
	content = content + savedWifiListResolver();
	content = content + "<div id=\"wifi-editor\" hx-boost=\"true\"></div>";

	if (request->hasHeader("hx-request")) {
		return request->beginResponse(200, "text/html", content);
	}

	String page = layoutResolver(layoutResolverArgs{
		page: "Wifi",
		content: content,
	});

	return request->beginResponse(200, "text/html", page);
}