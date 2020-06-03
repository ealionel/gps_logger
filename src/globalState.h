#ifndef GLOBAL_STATE_H_INCLUDED
#define GLOBAL_STATE_H_INCLUDED

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "lcdView.h"

#define LCD_LEFT_ARROW  0b01111111 // <-
#define LCD_RIGHT_ARROW 0b01111111 // ->
#define LCD_CUSTOM_CHECKMARK byte(0)

// Adresse EEPROM du paramètre logInterval (int=2bytes)
#define EEPROM_LOGINTERVAL_ADDR 0


class LCDViewManager;  // forward declaration

extern LiquidCrystal lcd;
extern LCDViewManager views;
extern unsigned char buttonState;
extern unsigned char lastButtonState;

#endif