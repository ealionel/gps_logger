#include <Arduino.h>
#include <GPSport.h>
#include <LiquidCrystal.h>
#include <NMEAGPS.h>
#include <SD.h>
#include <SPI.h>
#include <Streamers.h>

#include "buttons.h"
#include "lcdView.h"
#define TX_PIN 2
#define RX_PIN 3
NMEAGPS gps;
gps_fix fix;

#define VBAT_PIN A0  // Battery voltage pin

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

LCDView *currentView;

LCDView views[4] = {};

void setup() {
    Serial.begin(9600);
    gpsPort.begin(4800);

    pinMode(BP0_PIN, INPUT);
    pinMode(BP1_PIN, INPUT);
    pinMode(BPEN_PIN, INPUT);

    lcd.begin(8, 2);

    currentView = &views[0];
}

void loop() {
    auto pressedButton = readButton();

    switch (pressedButton) {
        case SW_1:
            lcd.clear();
            currentView = &(views[0]);
            break;
        case SW_2:
            lcd.clear();
            currentView = &(views[1]);
            break;
    }

    currentView->render(lcd);

    // while (gps.available( gpsPort )) {
    //     fix = gps.read();

    //     DEBUG_PORT.print(F("Location: "));
    //     if (fix.valid.location) {
    //         DEBUG_PORT.print(fix.latitude(), 6);
    //         DEBUG_PORT.print(',');
    //         DEBUG_PORT.print(fix.longitude(), 6);
    //         lcd.setCursor(0, 0);
    //         lcd.print(fix.latitude());
    //         lcd.setCursor(0, 1);
    //         lcd.print(fix.longitude());
    //     }

    //     DEBUG_PORT.print(F(", Altitude: "));
    //     if (fix.valid.altitude) DEBUG_PORT.print(fix.altitude());

    //     DEBUG_PORT.println();
    // }
}