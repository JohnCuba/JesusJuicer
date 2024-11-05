#include <ESPAsyncWebServer.h>

#pragma once

class ServerModule {
	private:
	AsyncWebServer server;
	void log(String);
	static AwsTemplateProcessor connectProcessor(String, String);
	void registerApRoutes(WifiModule);

	public:
	ServerModule();
	void onSetup(WifiModule);
};
