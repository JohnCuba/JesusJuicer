#include <ESPAsyncWebServer.h>
#include "logg.hpp"

#pragma once

class ServerModule {
	private:
	static ServerModule* pinstance_;
	Logg logg = Logg("SERVER");
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
