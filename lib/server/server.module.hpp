#include <ESPAsyncWebServer.h>

#pragma once

class ServerModule {
 private:
  static const char loggTag_[7];
  static AsyncWebServer server;

 public:
  static void onSetup();
  static void registerRoute(const char *uri, WebRequestMethodComposite method,
                            ArRequestHandlerFunction onRequest);
  static void registerRoute(const char *uri, WebRequestMethodComposite method,
                            ArRequestHandlerFunction onRequest,
                            ArUploadHandlerFunction onUpload);
};
