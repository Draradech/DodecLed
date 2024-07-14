#ifdef _DEFINE_RAM_VARS_
#define global_var
#else
#define global_var extern
#endif

global_var CRGB leds[NUMLEDS];
global_var int animation;
global_var double voltage;
