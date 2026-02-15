#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <TFT_eSPI.h>
#include <vector>
#include "Config.h"

// Access global display objects defined in the main file
extern TFT_eSPI tft;
extern TFT_eSprite hud;
extern TFT_eSprite options;

// Graphics functions
void drawHUD(unsigned long lastTime, int flags, int bombs);
void drawOptions(int optState, int optSelect, int difficulty);
String millisToTime(unsigned long ms);

#endif