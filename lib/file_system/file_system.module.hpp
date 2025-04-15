#include "logg.hpp"
#include "ArduinoJson.h"

#pragma once
#define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

class FileSystemModule
{
private:
	static const char loggTag_[3];

public:
	static void onSetup();
	static String readFile(String path);
	static void writeFile(String path, String content);
	static void writeFile(String path, JsonDocument json);
};