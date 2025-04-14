#include <Update.h>
#include "server.module.hpp"
#include "updater.module.hpp"
#include "api_config.hpp"

UpdaterModule* UpdaterModule::pinstance_{nullptr};

UpdaterModule* UpdaterModule::GetInstance() {
	if (pinstance_ == nullptr) {
		pinstance_ = new UpdaterModule();
	}

	return pinstance_;
}

ArRequestHandlerFunction UpdaterModule::updateRequestHandler() {
	return [=](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(200, RES_TYPE_TEXT, (Update.hasError()) ? RES_BODY_BAD : RES_BODY_OK);
		response->addHeader("Connection", "close");
		request->send(response);
		delay(100);
		ESP.restart();
	};
};

ArUploadHandlerFunction UpdaterModule::updateUploadHandler(int command) {
	return [=](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
		if (!index) {
			logg.info("Upload Start: " + String(filename));
		}

		if (len) {
			if (!Update.begin(UPDATE_SIZE_UNKNOWN, command) && !Update.isRunning()) {
				logg.info(Update.errorString());
			};

			if (Update.write(data, len) == len) {
				Serial.print(".");
			} else {
				logg.info(Update.errorString());
				Update.abort();
				request->send(500, RES_TYPE_TEXT, Update.errorString());
			}
		}

		if (final) {
			Serial.print("\n");
			if (Update.end(true)) {
				logg.info("Update success, rebooting...");
			} else {
				Update.printError(Serial);
			}

			logg.info("Upload Complete: " + String(filename) + ",size: " + String(index + len));
		}
	};
}

void UpdaterModule::registerServerRoutes(const char* fwVersion) {
	logg.info("updater server routes");

	ServerModule* server_module = ServerModule::GetInstance();

	server_module->registerRoute("/api/update/fw", HTTP_GET, [=](AsyncWebServerRequest *request) {
		request->send(200, RES_TYPE_TEXT, String(fwVersion).c_str());
	});

	server_module->registerRoute(
		"/api/update/fw",
		HTTP_POST,
		updateRequestHandler(),
		updateUploadHandler(U_FLASH)
	);

	server_module->registerRoute(
		"/api/update/fs",
		HTTP_POST,
		updateRequestHandler(),
		updateUploadHandler(U_SPIFFS)
	);
}

void UpdaterModule::onSetup(const char* fwVersion) {
	logg.info("start setup");

	registerServerRoutes(fwVersion);

	logg.info("end setup");
}