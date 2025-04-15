#pragma once

#include <ESPAsyncWebServer.h>
#include "logg.hpp"

class UpdaterModule
{
private:
	static const char loggTag_[8];

	static void registerServerRoutes(const char *fwVersion);
	static ArRequestHandlerFunction updateRequestHandler();
	static ArUploadHandlerFunction updateUploadHandler(int command);

public:
	static void onSetup(const char *fwVersion);
};
