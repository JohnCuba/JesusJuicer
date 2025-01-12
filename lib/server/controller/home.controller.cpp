#include "home.controller.hpp"
#include "resolver/resolve.layout.hpp"

AsyncWebServerResponse* homeController(AsyncWebServerRequest* request) {
		// TODO: Place here suggestion action or main metrics
	String content;

	if (request->hasHeader("hx-request")) {
		return request->beginResponse(200, "text/html", content);
	}

	String page = resolveLayout(resolveLayoutArgs{
		page: "Home",
		content,
	});

	return request->beginResponse(200, "text/html", page);
}