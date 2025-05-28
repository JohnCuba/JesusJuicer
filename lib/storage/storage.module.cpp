#include "storage.module.hpp"

#include "logger.h"
#include "nvs_flash.h"

const char StorageModule::loggTag_[8] = "STORAGE";

void StorageModule::setup() {
  ESP_LOGD(StorageModule::loggTag_, "start setup");
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  ESP_LOGD(StorageModule::loggTag_, "end setup");
};

bool StorageModule::setString(const char *space, const char *key,
                              const char *value) {
  nvs_handle_t handle;
  esp_err_t err;

  err = nvs_open(space, NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(StorageModule::loggTag_, "Error (%s) opening NVS handle!",
             esp_err_to_name(err));
    nvs_close(handle);
    return false;
  }

  err = nvs_set_str(handle, key, value);
  if (err != ESP_OK) {
    ESP_LOGE(StorageModule::loggTag_, "Failed to write string!");
    nvs_close(handle);
    return false;
  }

  nvs_close(handle);
  return true;
};

std::string StorageModule::getString(const char *space, const char *key,
                                     const std::string &fallback) {
  nvs_handle_t handle;
  esp_err_t err;

  err = nvs_open(space, NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(StorageModule::loggTag_, "Error (%s) opening NVS handle!",
             esp_err_to_name(err));
    nvs_close(handle);
    return fallback;
  }

  size_t required_size = 0;

  err = nvs_get_str(handle, key, NULL, &required_size);
  if (err != ESP_OK) {
    // TODO: pass key cernel panic
    ESP_LOGE(StorageModule::loggTag_, "failed to read string");
    nvs_close(handle);
    return fallback;
  }

  char *message = static_cast<char *>(malloc(required_size));
  if (!message) {
    ESP_LOGE(StorageModule::loggTag_, "memory allocation failed");
    nvs_close(handle);
    return fallback;
  }

  err = nvs_get_str(handle, key, message, &required_size);

  if (err != ESP_OK) {
    ESP_LOGE(StorageModule::loggTag_, "read string");
    free(message);
    nvs_close(handle);
    return fallback;
  }

  std::string result = message;

  free(message);
  nvs_close(handle);

  return result.empty() ? fallback : result;
}

bool StorageModule::deleteKey(const char *space, const char *key) {
  nvs_handle_t handle;
  esp_err_t err;

  err = nvs_open(space, NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(StorageModule::loggTag_, "Error (%s) opening NVS handle!",
             esp_err_to_name(err));
    nvs_close(handle);
    return false;
  }

  err = nvs_erase_key(handle, key);
  if (err != ESP_OK) {
    ESP_LOGE(StorageModule::loggTag_, "Error (%s) opening NVS handle!",
             esp_err_to_name(err));
    nvs_close(handle);
    return false;
  }

  return true;
}