#include <string>

#include "ArduinoJson.h"

#pragma once
#define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

class FileSystemModule {
 private:
  static const char loggTag_[3];

 public:
  static void onSetup();
  static std::string readFile(const char *path);
  static void writeFile(const char *path, std::string content);
  static void writeFile(const char *path, JsonDocument json);
};