// #include <Arduino.h>
// #include <LiquidCrystal.h>
// #include <SoftwareSerial.h>

// #include "buttons.h"

// // #include <NMEAGPS.h>
// #define TX_PIN 2
// #define RX_PIN 3
// // #include <GPSport.h>
// // NMEAGPS gps;
// // gps_fix fix;

// #include "TinyGPS++.h"
// TinyGPSPlus gps;

// SoftwareSerial ss(RX_PIN, TX_PIN);

// #define VBAT_PIN A0  // Battery voltage pin

// static LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

// void displayInfo();

// void setup() {
//     Serial.begin(9600);
//     ss.begin(4800);

//     pinMode(BP0_PIN, INPUT);
//     pinMode(BP1_PIN, INPUT);
//     pinMode(BPEN_PIN, INPUT);

//     lcd.begin(8, 2);
// }

// void displayInfo() {
//     Serial.print(F("Location: "));
//     if (gps.location.isValid()) {
//         Serial.print(gps.location.lat(), 6);
//         Serial.print(F(","));
//         Serial.print(gps.location.lng(), 6);
//         lcd.setCursor(0,1);
//         lcd.print("VALID GPS");
//     } else {
//         lcd.print("INVALID");
//         lcd.setCursor(1,0);
//         Serial.print(F("INVALID"));
//     }

//     Serial.print(F("  Date/Time: "));
//     if (gps.date.isValid()) {
//         Serial.print(gps.date.month());
//         Serial.print(F("/"));
//         Serial.print(gps.date.day());
//         Serial.print(F("/"));
//         Serial.print(gps.date.year());
//     } else {
//         Serial.print(F("INVALID"));
//     }

//     Serial.print(F(" "));
//     if (gps.time.isValid()) {
//         if (gps.time.hour() < 10) Serial.print(F("0"));
//         Serial.print(gps.time.hour());
//         Serial.print(F(":"));
//         if (gps.time.minute() < 10) Serial.print(F("0"));
//         Serial.print(gps.time.minute());
//         Serial.print(F(":"));
//         if (gps.time.second() < 10) Serial.print(F("0"));
//         Serial.print(gps.time.second());
//         Serial.print(F("."));
//         if (gps.time.centisecond() < 10) Serial.print(F("0"));
//         Serial.print(gps.time.centisecond());
//     } else {
//         Serial.print(F("INVALID"));
//     }

//     Serial.println();
// }


// void loop() {
//     int val = readButton();
//     lcd.setCursor(0, 0);
//     lcd.print(val);

//     while (ss.available() > 0) {
//         char data = ss.read();
//         Serial.print(data);
//         if (gps.encode(data)) {
//             displayInfo();
//         }
//     }

// }