#include "stdinc.h"
#include "snoise.h"

void animationStep(void);

void battery_low(void);
void anim02(void);
void anim03(void);
void anim04(void);
void anim05(void);
void anim06(void);

void (*animations[10])(void) =
{
  NULL,
  NULL,
  &anim02,
  &anim03,
  &anim04,
  &anim05,
  &anim06,
  &anim06,
  &anim06,
  &anim06
};

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

void battery_low(void)
{
  static int spinner;
  static int spincnt;
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

void anim02(void)
{
  static int phase;
  phase--;
  if(phase < 0) phase += NUMLEDS;
  for(int i = 0; i < NUMLEDS; i++)
  {
    leds[spiral[i]] = CHSV(((i + phase) % NUMLEDS) * 255 / NUMLEDS, 255, 255);
  }
}

void anim03(void)
{
  static int phase;
  static int bandpos;
  bandpos += 1;
  for(int i = 0; i < NUMLEDS; i++)
  {
    float ledpos;
    switch(phase)
    {
      case 1:
      {
        ledpos = positions[i].x;
        break;
      }
      case 2:
      {
        ledpos = positions[i].y;
        break;
      }
      case 3:
      {
        ledpos = positions[i].z;
        break;
      }
      default:
      {
        phase = 1;
      }
    }
    float dist = fabs(ledpos - bandpos);
    float bright = 20.0f - dist;
    if (bright  < 0.0f) bright = 0.0f;
    bright *= 12.5;
    leds[i] = CRGB(bright, bright, bright);
  }
  if (bandpos > 100)
  {
    phase %= 3;
    phase++;
    bandpos = -100;
  }
}

void anim04(void)
{
  static int slowdelay;
  static int phase;
  static int hue;
  if(slowdelay++ >= 2)
  {
    slowdelay = 0;
    phase++;
    phase %= NUMLEDS;
    if(phase % 10 == 0)
    {
      hue++;
      hue %= 256;
    }
    leds[spiral[phase]] = CHSV(hue, 255, 255);
  }
  fadeToBlackBy(leds, NUMLEDS, 25);
}

void anim05(void)
{
  static int huesteps;
  static int slowdelay;
  static int hue;

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
}

void anim06(void)
{
  static float phase = 1.7;
  for(int i = 0; i < NUMLEDS; i++)
  {
    float hue = snoise3(
      (positions[i].x + 2048 * sin(phase * 17)) / 512,
      (positions[i].y + 2048 * sin(phase * 19)) / 512,
      (positions[i].z + 2048 * sin(phase * 23)) / 512
    );
    hue = (hue / 0.8 + 1.0)  / 2.0;
    hue = LIMIT(hue, 0.0, 1.0);
    hue = hue * 255;
    leds[i] = CHSV(hue, 255, 255);
  }
  phase += 0.0001;
}

void animationStep(void)
{
  if(voltage < 10.5)
  {
    FastLED.setBrightness(40);
    battery_low();
  }
  else
  {
    FastLED.setBrightness(255);
    void (*anim_func)(void) = animations[animation];
    if (anim_func != NULL)
    {
      (*anim_func)();
    }
  }
  FastLED.show();  
}
