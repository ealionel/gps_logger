#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

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

#endif