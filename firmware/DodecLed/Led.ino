#define NUMLEDS 60

CRGB leds[NUMLEDS];
int mode = 1;
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
        phase--;
        if(phase < 0) phase += NUMLEDS;
        for(int i = 0; i < NUMLEDS; i++)
        {
          leds[spiral[i]] = CHSV(((i + phase) % NUMLEDS) * 255 / NUMLEDS, 255, 255);
        }
        break;
      }
      case 2:
      {
        phase++;
        phase %= NUMLEDS;
        if(phase % 10 == 0)
        {
          col++;
          col %= 256;
        }
        fadeToBlackBy(leds, NUMLEDS, 50);
        leds[spiral[phase]] = CHSV(col, 255, 255);
        break;
      }
      case 3:
      {
        if(slowdelay++ == 10)
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
      case 4:
      case 5:
      {
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
