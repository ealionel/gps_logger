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
#include "helper.h"
#include "globalState.h"


#define VBAT_PIN A0  // Battery voltage pin

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
ProgramContext context;
LCDViewManager views(context);
NMEAGPS gps;

ButtonId buttonState;
ButtonId lastButtonState;

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

    context.logger.init();

    File dir2 = SD.open("/LOGS");
    printDirectory(dir2, 0);

    context.logger.printIndexFile();

    LogIndexEntry* entries = context.logger.loadIndexFile();

    for (int i = 0; i < context.logger.getNbIndexEntries(); i++) {
        printLogIndexEntry(entries[i]);
    }

    delete [] entries;


    views.addView(new DefaultView);
    views.addView(new CoordinateView);
    views.selectView(0);


    buttonState = readButton();
    
    lcd.begin(8, 2);
}

void loop() {
    buttonState = readButton();

    onButtonPush(SW_1, []()->void {
        views.selectView(0);
    });

    onButtonPush(SW_2, []()->void {
        views.selectView(1);
    });

    while (gps.available(gpsPort)) {
        context.fix = gps.read();

        if (context.fix.valid.location) {
            context.logger.log(context.fix);
            lcd.setCursor(0, 1);
            lcd.print("OK");
        }
    }

    views.renderView();
    lastButtonState = buttonState;
}