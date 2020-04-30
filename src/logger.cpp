#include "logger.h"

GPSLogger::GPSLogger() {}

void GPSLogger::disable() { isLogging = false; }

void GPSLogger::enable() { isLogging = true; }

void GPSLogger::setLogFile(String path) { logFile = path; }

void GPSLogger::log(gps_fix fix) {
    if (isLogging) {
        File file = SD.open(logFile, FILE_WRITE);

        if (file) {
            writeCsvLine(file, fix);
        } else {
            Serial.print("LOGGER : Failed to open ");
            Serial.println(logFile);
        }
        file.close();
    }
}

void GPSLogger::writeCsvLine(File &file, gps_fix fix) {
    if (fix.valid.time) {
        file.print(fix.dateTime.date);
        file.print(F("/"));
        file.print(fix.dateTime.month);
        file.print(F("/"));
        file.print(fix.dateTime.year);
    }

    file.print(F(","));
    if (fix.valid.time) {
        file.print(fix.dateTime.hours);
        file.print(F(":"));
        file.print(fix.dateTime.minutes);
        file.print(F(":"));
        file.print(fix.dateTime.seconds);
    }
    file.print(F(","));
    if (fix.valid.location) {
        file.print(fix.latitude(), 6);
        file.print(F(", "));
        file.print(fix.longitude(), 6);
    }

    file.print(F(","));
    if (fix.valid.altitude) file.print(fix.altitude());

    file.print(F(","));
    if (fix.valid.speed) file.print(fix.speed_kph());

    file.println("");
}

// String GPSLogger::buildCSVLine(gps_fix fix) {}