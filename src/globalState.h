#ifndef GLOBAL_STATE_H_INCLUDED
#define GLOBAL_STATE_H_INCLUDED

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "lcdView.h"

#define LEFT_ARROW_LCD 0b01111111

#define EEPROM_INTERVAL_ADDR 0


class LCDViewManager;  // forward declaration

extern LiquidCrystal lcd;
extern LCDViewManager views;
extern unsigned char buttonState;
extern unsigned char lastButtonState;

#endif