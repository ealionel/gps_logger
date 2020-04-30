#pragma once

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <NMEAGPS.h>

class GPSLogger {
    public:
        GPSLogger();
        void log(gps_fix fix);
        void disable();
        void enable();
        void setLogFile(String path);

        void writeCsvLine(File &file, gps_fix fix);

    private:
        bool isLogging = false;
        String logFile;
        File currentFile;
};