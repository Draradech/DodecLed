#include <FastLED.h>

#define NUMLEDS 60
#define LIMIT(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

typedef struct
{
  float x;
  float y;
  float z;
} Vec3f;

extern Vec3f positions[NUMLEDS];
extern int spiral[NUMLEDS];

#include "snoise.h"
#include "ram.h"

void setupVoltage(void);
void setupWifi(void);
void setupUdp(void);
void setupWeb(void);
void setupLeds(void);
void setupOta(void);

void loopVoltage(void);
void loopWifi(void);
void loopUdp(void);
void loopWeb(void);
void loopLeds(void);
void loopOta(void);
