const char* ssid = "d12-wlan";
const char* passwd = "d12-wlan-d12";

void setupWifi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, passwd);
}

void loopWifi()
{
  
}
