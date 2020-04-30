#include "helper.h"



void printDirectory(File &dir, int numTabs) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            Serial.println("No more files");
            // no more files
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++) {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        } else {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

void printFile(String path) {
    File file = SD.open(path, FILE_READ);

    if (file) {
        while (file.available()) {
            Serial.write(file.read());
        }
    } else {
        Serial.print("Cannot open ");
        Serial.println(path);
    }
}