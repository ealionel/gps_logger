#include "logger.h"

#include "helper.h"

#define INDEX_FILE_NAME "index"

LogIndexEntry createLogIndexEntry(int id, String fileName, int nbLogs,
                                  String date) {
    LogIndexEntry entry = {id, fileName, nbLogs, date};

    return entry;
}

GPSLogger::GPSLogger() {}

void GPSLogger::init(String root) {
    this->root = root;

    if (SD.exists(root)) {
        File rootFile = SD.open(root);

        if (!rootFile.isDirectory()) {
            rootFile.close();
            SD.remove(root);
            Serial.println(F("Removing existing file..."));
        } else {
            rootFile.close();
        }
    }

    SD.mkdir(root);

    if (!SD.exists(getIndexPath())) {
        // Crée le fichier index s'il n'existe pas encore
        initIndexFile();
        Serial.print(F("No index file found, creating "));
        Serial.println(getIndexPath());
    }

    nbIndexEntries = countIndexEntries();
}

int GPSLogger::countIndexEntries() {
    File index = SD.open(getIndexPath(), FILE_READ);

    int nbEntries = 0;
    while (index.available()) {
        if (index.read() == '\n') nbEntries++;
    }
    index.close();
    return nbEntries;
}

int GPSLogger::getNbIndexEntries() { return nbIndexEntries; }

void GPSLogger::addIndexEntry(LogIndexEntry entry) {
    File index = SD.open(getIndexPath(), FILE_WRITE);

    index.print(getNbIndexEntries());
    index.print(F(","));
    index.print(entry.fileName);
    index.print(F(","));
    index.print(entry.nbLogs);
    index.print(F(","));
    index.print(entry.date);
    index.print(F("\n"));

    index.close();
}

void GPSLogger::clearDirectory() {
    File dir = SD.open(getLogPath());

    File entry;
    do {
        entry = dir.openNextFile();

        if (entry) {
            if (SD.remove(root + entry.name())) {
                Serial.println("Removed " + root + entry.name());
            } else {
                Serial.println("Coudln't remove " + root + entry.name());
                dir.close();
                break;
            }
        }

    } while (entry);

    initIndexFile();
    nbIndexEntries = 0;
}

void GPSLogger::disable() { isLogging = false; }

void GPSLogger::enable() { isLogging = true; }

void GPSLogger::setLogFile(String name) { logFile = name; }

void GPSLogger::setInterval(unsigned int interval) { logInterval = interval; }

String GPSLogger::getLogPath() { return root + logFile; }

String GPSLogger::getIndexPath() { return root + INDEX_FILE_NAME; }

size_t GPSLogger::dirCount() {}

void GPSLogger::log(gps_fix fix) {
    if (isLogging) {
        File file = SD.open(getLogPath(), FILE_WRITE);

        if (file) {
            writeCsvLine(file, fix);
        } else {
            Serial.print("LOGGER : Failed to open ");
            Serial.println(logFile);
        }
        file.close();
    }
}

void GPSLogger::writeFile(String filename, String line) {
    File file = SD.open(root + filename, FILE_WRITE);
    file.println(line);
    file.close();
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

void GPSLogger::printIndexFile() { printFile(getIndexPath()); }

void GPSLogger::initIndexFile() {
    File indexFile = SD.open(getIndexPath(), FILE_WRITE);
    indexFile.close();
    Serial.print("Initializing ");
    Serial.println(getIndexPath());
}