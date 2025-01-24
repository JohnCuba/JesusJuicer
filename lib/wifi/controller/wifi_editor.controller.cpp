#include "wifi_editor.controller.hpp"
#include "../server/resolver/layout.resolver.hpp"
#include "../resolver/wifi_editor.resolver.hpp"
#include "../resolver/saved_wifi_list.resolver.hpp"

AsyncWebServerResponse* wifiEditorController(AsyncWebServerRequest* request) {
	if(!request->hasParam("target")) {
		return request->beginResponse(400, "text/html", String("Provide target to edit"));
	}
	AsyncWebParameter* targetRaw = request->getParam("target");

	String content;

	content = content + wifiEditorResolver(targetRaw->value());

	if (request->hasHeader("hx-request")) {
		return request->beginResponse(200, "text/html", content);
	}

	content = savedWifiListResolver() + "<div id=\"wifi-editor\" hx-boost=\"true\">" + content + "</div>";

	String page = layoutResolver(layoutResolverArgs{
		page: "Wifi",
		content: content,
	});

	return request->beginResponse(200, "text/html", page);
}