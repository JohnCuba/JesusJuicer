#include <Update.h>
#include "server.module.hpp"
#include "updater.module.hpp"
#include "api_config.hpp"

const char UpdaterModule::loggTag_[8] = "UPDATER";

UpdaterModule *UpdaterModule::pinstance_{nullptr};

UpdaterModule *UpdaterModule::GetInstance()
{
	if (pinstance_ == nullptr)
	{
		pinstance_ = new UpdaterModule();
	}

	return pinstance_;
}

ArRequestHandlerFunction UpdaterModule::updateRequestHandler()
{
	return [=](AsyncWebServerRequest *request)
	{
		AsyncWebServerResponse *response = request->beginResponse(200, RES_TYPE_TEXT, (Update.hasError()) ? RES_BODY_BAD : RES_BODY_OK);
		response->addHeader("Connection", "close");
		request->send(response);
		delay(100);
		ESP.restart();
	};
};

ArUploadHandlerFunction UpdaterModule::updateUploadHandler(int command)
{
	return [=](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
	{
		if (!index)
		{
			Logg::info(UpdaterModule::loggTag_, "Upload Start: [%s]", filename);
		}

		if (len)
		{
			if (!Update.begin(UPDATE_SIZE_UNKNOWN, command) && !Update.isRunning())
			{
				Logg::error(UpdaterModule::loggTag_, Update.errorString());
			};

			if (Update.write(data, len) == len)
			{
				// TODO: make line logger
				Serial.print(".");
			}
			else
			{
				Logg::error(UpdaterModule::loggTag_, Update.errorString());
				Update.abort();
				request->send(500, RES_TYPE_TEXT, Update.errorString());
			}
		}

		if (final)
		{
			Serial.print("\n");
			if (Update.end(true))
			{
				Logg::info(UpdaterModule::loggTag_, "Update success, rebooting...");
			}
			else
			{
				Update.printError(Serial);
			}

			Logg::info(UpdaterModule::loggTag_, "Upload Complete: [%s],size: [%s]", String(filename), String(index + len));
		}
	};
}

void UpdaterModule::registerServerRoutes(const char *fwVersion)
{
	Logg::debug(UpdaterModule::loggTag_, "updater server routes");

	ServerModule *server_module = ServerModule::GetInstance();

	server_module->registerRoute("/api/update/fw", HTTP_GET, [=](AsyncWebServerRequest *request)
															 { request->send(200, RES_TYPE_TEXT, String(fwVersion).c_str()); });

	server_module->registerRoute(
			"/api/update/fw",
			HTTP_POST,
			updateRequestHandler(),
			updateUploadHandler(U_FLASH));

	server_module->registerRoute(
			"/api/update/fs",
			HTTP_POST,
			updateRequestHandler(),
			updateUploadHandler(U_SPIFFS));
}

void UpdaterModule::onSetup(const char *fwVersion)
{
	Logg::debug(UpdaterModule::loggTag_, "start setup");

	registerServerRoutes(fwVersion);

	Logg::debug(UpdaterModule::loggTag_, "end setup");
}