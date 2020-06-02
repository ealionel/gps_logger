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

DefaultView defaultView;
CoordinateView coordinateView;
IndexView indexView;

void setup() {
    Serial.begin(9600);
    gpsPort.begin(4800);

    pinMode(BP0_PIN, INPUT);
    pinMode(BP1_PIN, INPUT);
    pinMode(BPEN_PIN, INPUT);

    if (!SD.begin(SS_PIN)) {
        Serial.println(F("SD not ok"));
    }

    context.logger.init();

    File dir2 = SD.open("/LOGS");
    printDirectory(dir2, 0);

    context.logger.printIndexFile();

    views.addView(&defaultView);
    views.addView(&coordinateView);
    views.addView(&indexView);

    // views.addView(new SettingsView);
    views.selectView(0);


    buttonState = readButton();
    
    lcd.begin(8, 2);
    context.logger.setInterval(5);
    // context.logger.enable();
}


void loop() {
    buttonState = readButton();

    onButtonPush(SW_4, []()->void {
        views.selectNextView();
    });


    while (gps.available(gpsPort)) {
        context.fix = gps.read();
        context.logger.log(context.fix);
    }

    views.renderView();
    lastButtonState = buttonState;
}