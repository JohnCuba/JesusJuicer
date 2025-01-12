#include "home.controller.hpp"
#include "resolver/layout.resolver.hpp"

AsyncWebServerResponse* homeController(AsyncWebServerRequest* request) {
		// TODO: Place here suggestion action or main metrics
	String content;

	if (request->hasHeader("hx-request")) {
		return request->beginResponse(200, "text/html", content);
	}

	String page = layoutResolver(layoutResolverArgs{
		page: "Home",
		content,
	});

	return request->beginResponse(200, "text/html", page);
}