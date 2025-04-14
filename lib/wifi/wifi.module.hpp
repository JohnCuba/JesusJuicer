#include "logg.hpp"
#include "file_system.module.hpp"
#include "wifi_credentials.hpp"

#pragma once

class WifiModule
{
private:
	static WifiModule *pinstance_;
	static const char loggTag_[5];

	void createAP();
	bool connectToAP();
	void registerServerRoutes();

protected:
	WifiModule() {}
	~WifiModule() {}

public:
	static WifiModule *GetInstance();
	void onSetup();
};