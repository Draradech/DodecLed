#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <FastLED.h>

#define LIMIT(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

double voltageraw = 10.5;
double voltage = 10.5;
const double offset = 0.61; // device specific, calibration needed
int initVoltageCount = 100;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("setup wifi");
  setupWifi();
  Serial.println("setup udp");
  setupUdp();
  Serial.println("setup web");
  setupWeb();
  Serial.println("setup leds");
  setupLeds();
  Serial.println("setup ota");
  setupOta();
  Serial.println("setup done");
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
    voltageraw = 0.9 * voltageraw + 0.1 * (analogReadMilliVolts(36) * 0.001 * 11.0 + offset);
  }
  else
  {
    voltageraw = 0.999 * voltageraw + 0.001 * (analogReadMilliVolts(36) * 0.001 * 11.0 + offset);
  }
}
