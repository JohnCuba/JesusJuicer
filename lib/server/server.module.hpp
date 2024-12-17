#include <ESPAsyncWebServer.h>
#include "file_system.module.hpp"
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
};
