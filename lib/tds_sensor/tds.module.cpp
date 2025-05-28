#include "tds.module.hpp"

#include <ESPAsyncWebServer.h>

#include "api_config.hpp"
#include "logger.h"
#include "server.module.hpp"

int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++) bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0) {
    bTemp = bTab[(iFilterLen - 1) / 2];
  } else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}

const char TDSModule::loggTag_[4] = "TDS";

float TDSModule::getValue() {
  static unsigned long analogSampleTimepoint = millis();
  static unsigned int analogBuffer[sCount];
  static int analogBufferTemp[sCount];
  static unsigned int analogBufferIndex = 0;
  static unsigned int copyIndex = 0;

  static float averageVoltage = 0;
  static float rawTdsValue = 0;
  static float temperature = 25;

  if (millis() - analogSampleTimepoint > 40U) {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(sensorPin);

    analogBufferIndex++;
    if (analogBufferIndex == sCount) {
      analogBufferIndex = 0;
    }
  }

  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U) {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < sCount; copyIndex++) {
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];

      // read the analog value more stable by the median filtering algorithm,
      // and convert to voltage value
      averageVoltage =
          getMedianNum(analogBufferTemp, sCount) * (float)vRef / 4096.0;

      // temperature compensation formula: fFinalResult(25^C) =
      // fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
      // temperature compensation
      float compensationVoltage = averageVoltage / compensationCoefficient;

      // convert voltage value to tds value
      rawTdsValue = (133.42 * compensationVoltage * compensationVoltage *
                         compensationVoltage -
                     255.86 * compensationVoltage * compensationVoltage +
                     857.39 * compensationVoltage) *
                    0.5;
    }
  }

  if (rawTdsValue < 0 || rawTdsValue > 2000) return 0;

  return rawTdsValue;
}

void TDSModule::registerServerRoutes() {
  ESP_LOGD(TDSModule::loggTag_, "setup server routes");

  ServerModule::registerRoute(
      "/api/tds", HTTP_GET, [=](AsyncWebServerRequest *request) {
        request->send(200, RES_TYPE_TEXT, std::to_string(getValue()).c_str());
      });

  ServerModule::registerRoute("/api/tds", HTTP_OPTIONS,
                              [=](AsyncWebServerRequest *request) {
                                request->send(200, RES_TYPE_TEXT, RES_BODY_OK);
                              });
}

void TDSModule::onSetup() {
  ESP_LOGD(TDSModule::loggTag_, "start setup");

  TDSModule::registerServerRoutes();

  pinMode(sensorPin, INPUT);

  ESP_LOGD(TDSModule::loggTag_, "end setup");
}
