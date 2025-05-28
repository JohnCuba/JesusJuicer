#ifndef LOGGER
#define LOGGER

//** NOTES:
//***
//**   USE logger.h IN PLACE OF esp_log.h
//**
//**   1) this must be included in platformio.ini
//**       build_flags =
//**         -DCORE_DEBUG_LEVEL=5 */
//**
//**   2) in setup(), include the following after setting up Serial port
//**           Serial.setDebugOutput(true);  // this is important for some esp
//processors such as S2-mini
//**
//**           // Set global log level to to some level initially
//**           esp_log_level_set("*", GLOBAL_LOG_LEVEL);
//**
//**      then in subsequent code, you can use lines like the following to
// change the log level for each function, such as...
//**
//**           esp_log_level_set(TAG, ESP_LOG_ERROR);

// Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.1-632e0c2a/esp32/include/log/include/
#include <esp_log.h>
// Library/Arduino15/packages/esp32/hardware/esp32/3.0.7/cores/esp32/esp32-hal-log.h
#include <esp32-hal-log.h>

#undef CONFIG_LOG_MAXIMUM_LEVEL
#define CONFIG_LOG_MAXIMUM_LEVEL ESP_LOG_VERBOSE

#undef ESP_LOGE
#undef ESP_LOGW
#undef ESP_LOGI
#undef ESP_LOGD
#undef ESP_LOGV

#define ESP_LOGE(tag, format, ...)                                 \
  ESP_LOG_LEVEL_LOCAL(ESP_LOG_ERROR, tag,                          \
                      ARDUHAL_LOG_FORMAT(E, format) __VA_OPT__(, ) \
                          __VA_ARGS__)
#define ESP_LOGW(tag, format, ...)                                 \
  ESP_LOG_LEVEL_LOCAL(ESP_LOG_WARN, tag,                           \
                      ARDUHAL_LOG_FORMAT(W, format) __VA_OPT__(, ) \
                          __VA_ARGS__)
#define ESP_LOGI(tag, format, ...)                                 \
  ESP_LOG_LEVEL_LOCAL(ESP_LOG_INFO, tag,                           \
                      ARDUHAL_LOG_FORMAT(I, format) __VA_OPT__(, ) \
                          __VA_ARGS__)
#define ESP_LOGD(tag, format, ...)                                 \
  ESP_LOG_LEVEL_LOCAL(ESP_LOG_DEBUG, tag,                          \
                      ARDUHAL_LOG_FORMAT(D, format) __VA_OPT__(, ) \
                          __VA_ARGS__)
#define ESP_LOGV(tag, format, ...)                                 \
  ESP_LOG_LEVEL_LOCAL(ESP_LOG_VERBOSE, tag,                        \
                      ARDUHAL_LOG_FORMAT(V, format) __VA_OPT__(, ) \
                          __VA_ARGS__)

#undef ARDUHAL_LOG_FORMAT
#define ARDUHAL_LOG_FORMAT(letter, format)                                    \
  ARDUHAL_LOG_COLOR_##letter "[%s:%u] %s(): " format ARDUHAL_LOG_RESET_COLOR, \
      pathToFileName(__FILE__), __LINE__, __FUNCTION__

#endif