static bool done = false;
static int16_t rebootDelay = 500;

void setupOta()
{
  ArduinoOTA.setHostname("WifiLed");
  ArduinoOTA.onEnd([](){done = true;});
  ArduinoOTA.begin();
  Serial.println("OTA setup");
}

void loopOta()
{
  ArduinoOTA.handle();
  if(done)
  {
    if(--rebootDelay == 0)
    {
      ESP.restart();
    }
  }
}
