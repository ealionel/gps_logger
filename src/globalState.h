#ifndef GLOBAL_STATE_H_INCLUDED
#define GLOBAL_STATE_H_INCLUDED

#include "lcdView.h"
#include <Arduino.h>

class LCDViewManager; // forward declaration

extern LiquidCrystal lcd;
extern LCDViewManager views;
extern unsigned char buttonState;
extern unsigned char lastButtonState;

#endif