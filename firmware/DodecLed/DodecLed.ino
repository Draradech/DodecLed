#include "stdinc.h"

void setup(void)
{
  setupVoltage();
  setupWifi();
  setupUdp();
  setupWeb();
  setupLeds();
  setupOta();
}

void loop(void)
{
  loopVoltage();
  loopWifi();
  loopUdp();
  loopWeb();
  loopLeds();
  loopOta();
}
