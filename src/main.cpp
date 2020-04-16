#include <Arduino.h>
#include <LiquidCrystal.h>

#include <Streamers.h>

#include "buttons.h"

#include <SPI.h>
#include <SD.h>

#include <NMEAGPS.h>
#define TX_PIN 2
#define RX_PIN 3
#include <GPSport.h>
NMEAGPS gps;
gps_fix fix;

#define VBAT_PIN A0  // Battery voltage pin


uint8_t lcdView;

static LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

void setup() {
    Serial.begin(9600);
    gpsPort.begin(4800);

    pinMode(BP0_PIN, INPUT);
    pinMode(BP1_PIN, INPUT);
    pinMode(BPEN_PIN, INPUT);

    lcd.begin(8, 2);
}

void loop() {
    // int val = readButton();
    // lcd.print(val);
    // lcd.setCursor(0, 0);

    while (gps.available( gpsPort )) {
        fix = gps.read();

        DEBUG_PORT.print(F("Location: "));
        if (fix.valid.location) {
            DEBUG_PORT.print(fix.latitude(), 6);
            DEBUG_PORT.print(',');
            DEBUG_PORT.print(fix.longitude(), 6);
            lcd.setCursor(0, 0);
            lcd.print(fix.latitude());
            lcd.setCursor(0, 1);
            lcd.print(fix.longitude());
        }

        DEBUG_PORT.print(F(", Altitude: "));
        if (fix.valid.altitude) DEBUG_PORT.print(fix.altitude());

        DEBUG_PORT.println();
    }
}