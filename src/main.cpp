#include <Arduino.h>
#include <LiquidCrystal.h>
#include <NMEAGPS.h>
#include <SD.h>
#include <SPI.h>
#include <Streamers.h>

#define TX_PIN 2
#define RX_PIN 3

#include <GPSport.h>

#include "buttons.h"
#include "lcdView.h"
#include "logger.h"

NMEAGPS gps;
gps_fix fix;

#define VBAT_PIN A0  // Battery voltage pin

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
LCDViewManager views(lcd);

File root;
GPSLogger logger;

void printDirectory(File &dir, int numTabs) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            Serial.println("No more files");
            // no more files
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++) {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        } else {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

void printFile(String path) {
    File file = SD.open(path, FILE_READ);

    if (file) {
        while (file.available()) {
            Serial.write(file.read());
        }
    } else {
        Serial.print("Cannot open ");
        Serial.println(path);
    }
}

void setup() {
    Serial.begin(9600);
    gpsPort.begin(4800);

    pinMode(BP0_PIN, INPUT);
    pinMode(BP1_PIN, INPUT);
    pinMode(BPEN_PIN, INPUT);

    if (!SD.begin(SS_PIN)) {
        Serial.println("SD Card failed");
    } else {
        Serial.println("SD Card Initialized");
    }

    views.addView(new DefaultView);
    views.addView(new CoordinateView);
    views.selectView(0);

    SD.mkdir("/test");
    logger.setLogFile("/test/logtest.txt");
    logger.enable();

    printFile("/test/logtest.txt");

    gpsPort.listen();

    lcd.begin(8, 2);
}

void loop() {
    auto pressedButton = readButton();

    switch (pressedButton) {
        case SW_1:
            views.selectView(0);
            break;
        case SW_2:
            views.selectView(1);
            break;
        case SW_3:
            if (SD.exists("/test/logtest.txt")) {
                SD.remove("/test/logtest.txt");
            }
            break;
    }

    views.renderView();

    while (gps.available(gpsPort)) {
        Serial.println("gps data available");
        fix = gps.read();

        if (fix.valid.location) {
            logger.log(fix);
            lcd.setCursor(0, 1);
            lcd.print("OK");
        }

        // DEBUG_PORT.print(F("Location: "));
        // if (fix.valid.location) {
        //     DEBUG_PORT.print(fix.latitude(), 6);
        //     DEBUG_PORT.print(',');
        //     DEBUG_PORT.print(fix.longitude(), 6);
        //     lcd.setCursor(0, 0);
        //     lcd.print(fix.latitude());
        //     lcd.setCursor(0, 1);
        //     lcd.print(fix.longitude());
        // }

        // DEBUG_PORT.print(F(", Altitude: "));
        // if (fix.valid.altitude) DEBUG_PORT.print(fix.altitude());

        // DEBUG_PORT.println();
        Serial.println("");
    }
}