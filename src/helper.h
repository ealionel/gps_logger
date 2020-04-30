#pragma once

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <NMEAGPS.h>
#include "logger.h"

typedef struct {
    GPSLogger logger;
    gps_fix fix;
} ProgramContext;

void printDirectory(File &dir, int numTabs);
void printFile(String path);