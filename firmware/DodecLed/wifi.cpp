#include "stdinc.h"

#include <WiFi.h>

const char* ssid = "d12-wlan";
const char* passwd = "d12-wlan-d12";

void setupWifi(void)
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, passwd);
}

void loopWifi(void)
{
  
}
