#include <Preferences.h>

#include "wifi.module.hpp"
#include "server.module.hpp"
#include "telegram.module.hpp"
#include "api_config.hpp"

#define BOT_TOKEN "7646562962:AAGCvoHmCU3sNAY1MLvYjV9UEhkPn042Itw"
#define STORE_NAMESPACE "telegram"

const char TelegramModule::loggTag_[9] = "TELEGRAM";

UniversalTelegramBot TelegramModule::bot_{BOT_TOKEN, wifiSecureClient};

std::string TelegramModule::getChatId()
{
	Preferences preferences;
	if (!preferences.begin(STORE_NAMESPACE, true))
	{
		Logg::error(TelegramModule::loggTag_, "Error open preferences key: %s", STORE_NAMESPACE);
		return "";
	}

	std::string chat_id = preferences.getString("chat_id", "").c_str();

	preferences.end();

	return chat_id;
}
void TelegramModule::setChatId(std::string value)
{
	Preferences preferences;
	if (!preferences.begin(STORE_NAMESPACE))
	{
		Logg::error(TelegramModule::loggTag_, "Error open preferences key: %s", STORE_NAMESPACE);
		return;
	}

	preferences.putString("chat_id", value.c_str());

	preferences.end();
}

void TelegramModule::registerServerRoutes()
{
	Logg::debug(TelegramModule::loggTag_, "setup server routes");

	ServerModule::registerRoute(
			"/api/telegram/chat_id",
			HTTP_GET,
			[=](AsyncWebServerRequest *request)
			{
				request->send(200, RES_TYPE_TEXT, TelegramModule::getChatId().c_str());
			});

	ServerModule::registerRoute(
			"/api/telegram/chat_id",
			HTTP_POST,
			[=](AsyncWebServerRequest *request)
			{
				if (!request->hasParam("chat_id", true))
				{
					return request->send(422, RES_TYPE_TEXT, "provide chat_id");
				}

				std::string chat_id = request->getParam("chat_id", true)->value().c_str();
				TelegramModule::setChatId(chat_id);

				request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
			});

	ServerModule::registerRoute(
			"/api/telegram/ping",
			HTTP_POST,
			[=](AsyncWebServerRequest *request)
			{
				TelegramModule::sendMessage("ping");
				request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
			});
}

void TelegramModule::onSetup()
{
	Logg::debug(TelegramModule::loggTag_, "start setup");

	wifiSecureClient.setCACert(TELEGRAM_CERTIFICATE_ROOT);

	TelegramModule::registerServerRoutes();

	Logg::debug(TelegramModule::loggTag_, "end setup");
}

void TelegramModule::sendMessage(std::string message)
{
	bot_.sendSimpleMessage(TelegramModule::getChatId().c_str(), message.c_str(), "");
}