#include "helper.h"

void printDirectory(File dir, int numTabs) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            Serial.println(F("No more files"));
            // no more files
            // dir.rewindDirectory();
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++) {
            Serial.print(F("\t"));
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println(F("/"));
            printDirectory(entry, numTabs + 1);
        } else {
            // files have sizes, directories do not
            Serial.print(F("\t\t"));
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
        // Serial.print("printFile: Cannot open ");
        // Serial.println(path);
    }

    file.close();
}

String twoDigit(int digit) {
    return digit < 10 ? "0" + String(digit) : String(digit);
}

void formatDate(char buffer[9], gps_fix fix) {
    if (fix.valid.date) {
        snprintf(buffer, 9, "%d/%d/%d", fix.dateTime.day,
                 fix.dateTime.month, fix.dateTime.year);
    } else {
        buffer[0] = '\0';
    }
}

void formatTime(char buffer[9], gps_fix fix) {
    if (fix.valid.time) {
        snprintf(buffer, 9, "%d:%d:%d", fix.dateTime.hours,
                 fix.dateTime.minutes, fix.dateTime.seconds);
    } else {
        buffer[0] = '\0';
    }
}

String secToMin(int seconds) {
    return String(seconds / 60) + F("mn ") + (seconds % 60) + F("s");
}

void comma(File& file) { file.print(','); }

template <typename F>
void setInterval(int interval, int currentTime, F& callback) {}