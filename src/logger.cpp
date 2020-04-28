#include "logger.h"

GPSLogger::GPSLogger() {}

void GPSLogger::disable() { isLogging = false; }

void GPSLogger::enable() { isLogging = true; }

void GPSLogger::setLogFile(String path) { logFile = path; }

void GPSLogger::log(gps_fix fix) {
    if (isLogging) {
        File file = SD.open(logFile, FILE_WRITE);


        if (file) {
            Serial.println("Logging...");
            file.print(F("Location: "));
            if (fix.valid.location) {
                file.println(fix.latitude(), 6);
                file.print(F(", "));
                file.print(fix.longitude(), 6);
            }

            file.print(F(", Altitude: "));
            if (fix.valid.altitude) file.print(fix.altitude());

            file.print(F(", Time: "));
            if (fix.valid.time) {
                file.print(fix.dateTime.day);
                file.print(F("/"));
                file.print(fix.dateTime.date);
                file.print(F("/"));
                file.print(fix.dateTime.year);

                file.print(F(" "));

                file.print(fix.dateTime.hours);
                file.print(F(":"));
                file.print(fix.dateTime.minutes);
                file.print(F(":"));
                file.print(fix.dateTime.seconds);
            }

            file.print(F(", Speed: "));
            if (fix.valid.speed) {
                file.print(fix.speed_kph());
            }

        } else {
            Serial.print("LOGGER : Failed to open ");
            Serial.println(logFile);
        }

        file.close();
    }
}

// String GPSLogger::buildCSVLine(gps_fix fix) {}