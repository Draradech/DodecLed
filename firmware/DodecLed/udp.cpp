#include "stdinc.h"

#include <WiFiUdp.h>

static WiFiUDP udp;

void setupUdp(void)
{
  udp.begin(2323);
}

void loopUdp(void)
{
  int cb = udp.parsePacket();
  if (cb == NUMLEDS * 3)
  {
    udp.read((char*)leds, cb);
    animation = 1;
  }
}
