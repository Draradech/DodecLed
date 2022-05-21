#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <FastLED.h>

double voltageraw = 10.0;
double voltage = 10.0;

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

  voltageraw = 0.999 * voltageraw + 0.001 * analogReadMilliVolts(34) * 0.001 * 11.0;
}
