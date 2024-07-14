#include "stdinc.h"

const double offset = 0.61; // device specific, calibration needed
static double voltageraw = 10.5;
static int initVoltageCount = 100;

static unsigned long next;

void setupVoltage(void)
{
  voltage = 10.5;
  next = micros();
}

void loopVoltage(void)
{
  if(initVoltageCount > 80)
  {
    voltageraw = 0.9 * voltageraw + 0.1 * (analogRead(36) * 0.001 * 11.0 + offset);
  }
  else
  {
    voltageraw = 0.999 * voltageraw + 0.001 * (analogRead(36) * 0.001 * 11.0 + offset);
  }

  unsigned long now = micros();
  if(((signed long)(now - next)) > 0)
  {
    next += 20000;
    if(initVoltageCount > 0)
    {
      initVoltageCount--;
      if(initVoltageCount > 90)
      {
        voltage = 0.5 * voltage + 0.5 * voltageraw;
      }
      else
      {
        voltage = 0.9 * voltage + 0.1 * voltageraw;
      }
    }
    else
    {
      voltage = 0.999 * voltage + 0.001 * voltageraw;
    }
  }
}
