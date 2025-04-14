#include <ESPAsyncWebServer.h>
#include "logg.hpp"

#pragma once

class ServerModule
{
private:
	static ServerModule *instance_;
	static const char loggTag_[7];
	AsyncWebServer server;

protected:
	ServerModule();
	~ServerModule();

public:
	static ServerModule *GetInstance();
	void onSetup();
	void registerRoute(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);
	void registerRoute(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload);
};
