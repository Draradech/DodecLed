#include "snoise.h"
#define NUMLEDS 60

typedef struct
{
  float x;
  float y;
  float z;
} Vec3f;

CRGB leds[NUMLEDS];
int mode = 6;
unsigned long last;

int spiral[NUMLEDS] =
{
   0, 1, 2, 3, 4,
  21,20,16,15,11,10, 6, 5,26,25,
  29,22,23,24,17,18,19,12,13,14, 7, 8, 9,27,28,
  49,48,47,44,43,42,39,38,37,59,58,57,54,53,52,
  51,45,46,40,41,35,36,55,56,50,
  31,30,34,33,32,
};

Vec3f positions[NUMLEDS] =
{
  { 21.5f, -81.3f,  29.5f},
  { 34.7f, -81.3f, -11.3f},
  {  0.0f, -81.3f, -36.5f},
  {-34.7f, -81.3f, -11.3f},
  {-21.5f, -81.3f,  29.5f},
  //  5
  {-52.3f, -62.8f, -35.6f},
  {-17.6f, -62.8f, -60.8f},
  {-17.6f, -26.3f, -83.3f},
  {-52.3f,  -3.7f, -72.1f},
  {-73.8f, -26.3f, -42.5f},
  // 10
  { 17.6f, -62.8f, -60.8f},
  { 52.3f, -62.8f, -35.6f},
  { 73.8f, -26.3f, -42.5f},
  { 52.3f,  -3.7f, -72.1f},
  { 17.6f, -26.3f, -83.3f},
  // 15
  { 63.2f, -62.8f,  -2.0f},
  { 50.0f, -62.8f,  38.8f},
  { 63.2f, -26.3f,  57.1f},
  { 84.7f,  -3.7f,  27.5f},
  { 84.7f, -26.3f,  -9.0f},
  // 20
  { 21.5f, -62.8f,  59.5f},
  {-21.5f, -62.8f,  59.5f},
  {-34.7f, -26.3f,  77.8f},
  {  0.0f,  -3.7f,  89.1f},
  { 34.7f, -26.3f,  77.8f},
  // 25
  {-50.0f, -62.8f,  38.8f},
  {-63.2f, -62.8f,  -2.0f},
  {-84.7f, -26.3f,  -9.0f},
  {-84.7f,  -3.7f,  27.5f},
  {-63.2f, -26.3f,  57.1f},
  // 30
  {-34.7f,  81.3f,  11.3f},
  {-21.5f,  81.3f, -29.5f},
  { 21.5f,  81.3f, -29.5f},
  { 34.7f,  81.3f,  11.3f},
  {  0.0f,  81.3f,  36.5f},
  // 35
  { 63.2f,  62.8f,   2.0f},
  { 50.0f,  62.8f, -38.8f},
  { 63.2f,  26.3f, -57.1f},
  { 84.7f,   3.7f, -27.5f},
  { 84.7f,  26.3f,   9.0f},
  // 40
  { 17.6f,  62.8f,  60.8f},
  { 52.3f,  62.8f,  35.6f},
  { 73.8f,  26.3f,  42.5f},
  { 52.3f,   3.7f,  72.1f},
  { 17.6f,  26.3f,  83.3f},
  // 45
  {-52.3f,  62.8f,  35.6f},
  {-17.6f,  62.8f,  60.8f},
  {-17.6f,  26.3f,  83.3f},
  {-52.3f,   3.7f,  72.1f},
  {-73.8f,  26.3f,  42.5f},
  // 50
  {-50.0f,  62.8f, -38.8f},
  {-63.2f,  62.8f,   2.0f},
  {-84.7f,  26.3f,   9.0f},
  {-84.7f,   3.7f, -27.5f},
  {-63.2f,  26.3f, -57.1f},
  // 55
  { 21.5f,  62.8f, -59.5f},
  {-21.5f,  62.8f, -59.5f},
  {-34.7f,  26.3f, -77.8f},
  {  0.0f,   3.7f, -89.1f},
  { 34.7f,  26.3f, -77.8f},
};

void setupLeds()
{
  FastLED.addLeds<WS2812B, 2, GRB>(leds, NUMLEDS);
  FastLED.setBrightness(0);
  last = micros();
}

void loopLeds()
{
  unsigned long now = micros();
  while(((signed long)(now - last)) > 0)
  {
    animationStep();
    last += 20000;
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

int phase;
int spinner;
int spincnt;
int col;
int hue = 0;
int huesteps = 0;
int slowdelay;
int xoff = 2048;

void animationStep()
{
  if(voltage < 10.5)
  {
    FastLED.setBrightness(40);
    for(int i = spinner; i < NUMLEDS; i+=5)
    {
      leds[i] = CRGB::Black;
    }
    spincnt++;
    spincnt %= 10;
    if(!spincnt)
    {
      spinner++;
      spinner %= 5;
    }
    for(int i = spinner; i < NUMLEDS; i+=5)
    {
      leds[i] = CRGB::Red;
    }
  }
  else
  {
    FastLED.setBrightness(255);
    switch(mode)
    {
      case 1:
      {
        break;
      }
      case 2:
      {
        phase--;
        if(phase < 0) phase += NUMLEDS;
        for(int i = 0; i < NUMLEDS; i++)
        {
          leds[spiral[i]] = CHSV(((i + phase) % NUMLEDS) * 255 / NUMLEDS, 255, 255);
        }
        break;
      }
      case 3:
      {
        hue += 1;
        for(int i = 0; i < NUMLEDS; i++)
        {
          float pos;
          switch(phase)
          {
            case 1:
            {
              pos = positions[i].x;
              break;
            }
            case 2:
            {
              pos = positions[i].y;
              break;
            }
            case 3:
            {
              pos = positions[i].z;
              break;
            }
            default:
            {
              phase = 1;
            }
          }
          float dist = fabs(pos - hue);
          float bright = 20.0f - dist;
          if (bright  < 0.0f) bright = 0.0f;
          bright *= 12.5;
          leds[i] = CRGB(bright, bright, bright);
        }
        if (hue > 100)
        {
          phase %= 3;
          phase++;
          hue = -100;
        }
        break;
      }
      case 4:
      {
        if(slowdelay++ >= 2)
        {
          slowdelay = 0;
          phase++;
          phase %= NUMLEDS;
          if(phase % 10 == 0)
          {
            col++;
            col %= 256;
          }
          leds[spiral[phase]] = CHSV(col, 255, 255);
        }
        fadeToBlackBy(leds, NUMLEDS, 25);
        break;
      }
      case 5:
      {
        if(slowdelay++ >= 10)
        {
          slowdelay = 0;
          int l = random16(NUMLEDS);
          leds[l] = CHSV(hue, 128 + random8(128), 255);
          l = random16(NUMLEDS);
          leds[l] = CRGB::Black;
          l = random16(NUMLEDS);
          leds[l] = CRGB::Black;
        
          huesteps++;
          if(huesteps == 10)
          {
            huesteps = 0;
            hue = (hue + 1) % 256;
          }
        }
        break;
      }
      case 6:
      {
        for(int i = 0; i < NUMLEDS; i++)
        {
          float hue = snoise3((positions[i].x + xoff) / 384, (positions[i].y + 2048) / 384, (positions[i].z + 2048) / 384);
          hue = (hue / 0.8 + 1.0)  / 2.0;
          hue = LIMIT(hue, 0.0, 1.0);
          hue = hue * 255;
          leds[i] = CHSV(hue, 255, 255);
        }
        xoff++;
        break;
      }
      default:
      {
        for(int i = 0; i < NUMLEDS; i++)
        {
          leds[i] = CRGB::Black;
        }
      }
    }
  }
  FastLED.show();  
}
