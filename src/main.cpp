#include <Arduino.h>
#include <LiquidCrystal.h>
#include <NMEAGPS.h>
#include <SD.h>
#include <SPI.h>
#include <Streamers.h>
// #include <EEPROM.h>

#define TX_PIN 2
#define RX_PIN 3

#include <GPSport.h>

#include "globalState.h"
#include "buttons.h"

ButtonId buttonState;
ButtonId lastButtonState;

#include "lcdView.h"
#include "helper.h"
#include "logger.h"
#include "command.h"

#define VBAT_PIN A0  // Battery voltage pin

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
ProgramContext context;
LCDViewManager views(context);
NMEAGPS gps;

// --- Views initialization ---

DefaultView defaultView;
CoordinateView coordinateView;
IndexView indexView;
SettingsView settingsView;
NewLogView newLogView;

// ----------------------------

char cmd_buffer[17];

// --- Custom Characters Initialization ---

// Represents : ✓
uint8_t CUSTOM_CHECKMARK[] = {0x00, 0x00, 0x01, 0x02, 0x14, 0x08, 0x00, 0x00};

void setup() {
// EEPROM RESET CODE
//    for (int i = 0 ; i < EEPROM.length() ; i++) {
//     EEPROM.write(i, 0);
//   }
    Serial.begin(115200);
    gpsPort.begin(4800);

    pinMode(BP0_PIN, INPUT);
    pinMode(BP1_PIN, INPUT);
    pinMode(BPEN_PIN, INPUT);

    lcd.createChar(0, CUSTOM_CHECKMARK);

    if (!SD.begin(SS_PIN)) {
        context.logger.sdFailed = true;
    }

    context.logger.init();

    Serial.println("i"); // Serial initialization token

    // Serial.println(context.logger.getIndexPath());
    // Serial.println(context.logger.getLogPath());
    // File dir2 = SD.open("/LOGS");
    // printDirectory(dir2, 0);

    // context.logger.clearDirectory();

    // context.logger.printIndexFile();

    views.addView(&defaultView);
    views.addView(&coordinateView);
    views.addView(&indexView);
    views.addView(&newLogView);
    views.addView(&settingsView);

    views.selectView(0);

    buttonState = readButton();
    
    lcd.begin(8, 2);
}

int8_t input;

void loop() {
    buttonState = readButton();


    onButtonPush(SW_4, []() {
        views.selectNextView();
    });

    while (Serial.available()) {
        // Serial.flush();
        // size_t n = readBytesStringUntil(&Serial, '\n', cmd_buffer, 16);
        // String s = Serial.readStringUntil('\n');

        input = Serial.read();
        // Serial.println(input);
        handleCommandMinimal(input);
    }


    while (gps.available(gpsPort)) {
        context.fix = gps.read();
        context.logger.log(context.fix);

        if (context.logger.getNbIndexEntries() == 0 && context.fix.valid.date && context.fix.valid.time) {
            // Permet d'initialiser le fichier d'index
            context.logger.newLogFile(context.fix);
        }
    }

    views.renderView();
    lastButtonState = buttonState;
}