#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <FastLED.h>

double voltageraw = 10.5;
double voltage = 10.5;
const double offset = 0.61; // device specific, calibration needed
int initVoltageCount = 100;

void setup(void)
{
  setupWifi();
  setupUdp();
  setupWeb();
  setupLeds();
  setupOta();
}

void loop(void)
{
  loopWifi();
  loopUdp();
  loopWeb();
  loopLeds();
  loopOta();

  if(initVoltageCount > 80)
  {
    voltageraw = 0.9 * voltageraw + 0.1 * (analogReadMilliVolts(34) * 0.001 * 11.0 + offset);
  }
  else
  {
    voltageraw = 0.999 * voltageraw + 0.001 * (analogReadMilliVolts(34) * 0.001 * 11.0 + offset);
  }
}
