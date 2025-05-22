#include <string>

#pragma once

class StorageModule {
 private:
  static const char loggTag_[8];

 public:
  static void setup();
  static bool setString(const char *space, const char *key, const char *value);
  static std::string getString(const char *space, const char *key,
                               const std::string &fallback);
  static bool deleteKey(const char *space, const char *key);
};
