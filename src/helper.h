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

void printDirectory(File dir, int numTabs);
void printFile(String path);

// Ajoute un "0" en préfixe si le nombre ne possède qu'un chiffre
String twoDigit(int digit);

// Formate une date d'un gps_fix en JJ/MM/AAAA
String formatDate(gps_fix fix, String fallbackValue = "");

// Formate un temps d'un gps_fix en HH:MM:SS 
String formatTime(gps_fix fix, String fallbackValue = "");

// Converti les secondes en une chaine de caractère représentant les minutes
String secToMin(int seconds);

// Technique pour réduire la mémoire flash utilisée
// Économise 1 byte au lieu de faire file.print(F(","))
void comma(File& file);

template <typename F>
void setInterval(int time, F& callback);

#endif