#include "stdinc.h"
#include "snoise.h"

void animationStep(void);

static unsigned long next;
void setupLeds(void)
{
  FastLED.addLeds<WS2812B, 2, GRB>(leds, NUMLEDS);
  FastLED.setBrightness(0);
  next = micros();
  animation = 6;
}

void loopLeds(void)
{
  unsigned long now = micros();
  if(((signed long)(now - next)) > 0)
  {
    animationStep();
    next += 20000;
  }
}

static int phase;
static int spinner;
static int spincnt;
static int col;
static int hue = 0;
static int huesteps = 0;
static int slowdelay;
static int xoff = 2048;

void animationStep(void)
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
    switch(animation)
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
