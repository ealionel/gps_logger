#include "logger.h"
#include "helper.h"
#include "globalState.h"
#include <EEPROM.h>

#define INDEX_FILE_NAME "INDEX"

LogIndexEntry createLogIndexEntry(uint8_t id, String fileName, String date) {
    LogIndexEntry entry = {id, fileName, date};
    return entry;
}

void printLogIndexEntry(LogIndexEntry entry) {
    Serial.println("id\t" + String(entry.id, 10));
    Serial.println("name\t" + entry.fileName);
    Serial.println("date\t" + entry.date);
}

GPSLogger::GPSLogger() {}

void GPSLogger::init(String root) {
    if (sdFailed) return;

    int eepromInterval;
    EEPROM.get(EEPROM_LOGINTERVAL_ADDR, eepromInterval);

    if (eepromInterval != 0) {
        // Dans le cas où l'EEPROM ne serait pas initialisé
        // Ne devrait jamais arriver
        logInterval = eepromInterval;
    }

    this->root = root;

    if (SD.exists(root)) {
        File rootFile = SD.open(root);

        if (!rootFile.isDirectory()) {
            rootFile.close();
            SD.remove(root);
            // Serial.println(F("Removing existing file..."));
        } else {
            rootFile.close();
        }
    } else {
        SD.mkdir(root);
    }

    if (!SD.exists(getIndexPath())) {
        // Crée le fichier index s'il n'existe pas encore
        initIndexFile();
        // Serial.print(F("No index file found, creating "));
        Serial.println(getIndexPath());
    }

    nbIndexEntries = countIndexEntries();

    if (nbIndexEntries == 0) {
        // Si aucun fichier de log n'est présent on en crée au moins un
        // Serial.println("New log file");
        newLogFile((views.context)->fix);
    } else {
        logFile = getNbIndexEntries() - 1;
    }
}

int GPSLogger::countIndexEntries() {
    if (sdFailed) return 0;

    File index = SD.open(getIndexPath(), FILE_READ);

    int nbEntries = 0;
    while (index.available()) {
        if (index.read() == '\n')
            nbEntries++;
    }
    index.close();
    return nbEntries;
}

int GPSLogger::getNbIndexEntries() { return nbIndexEntries; }

void GPSLogger::addIndexEntry(LogIndexEntry entry) {
    if (sdFailed) return;

    File index = SD.open(getIndexPath(), FILE_WRITE);

    index.print(entry.id);
    comma(index);
    index.print(entry.fileName);
    comma(index);
    index.println(entry.date);

    index.close();

    nbIndexEntries++;
}

void GPSLogger::clearDirectory() {
    if (sdFailed) return;

    File dir = SD.open(root);

    while (true) {
        File entry = dir.openNextFile();

        if (!entry) {
            Serial.println("Done");
            break;
        }

        String fileName = root + entry.name();

        if (SD.remove(fileName)) {
            Serial.print(F("Removed "));
            Serial.print(fileName);
            Serial.println();
        } else {
            Serial.print(F("Couldn't remove "));
            Serial.print(fileName);
            Serial.println();
            break;
        }
        entry.close();
    }

    // dir.close();

    // Refait plus ou moins comme init() :
    initIndexFile();    // Recrée le fichier index
    nbIndexEntries = 0; // Remet le nombre d'entrées à 0
    newLogFile((views.context)->fix);       // Recrée un nouveau fichier de log initial
}

void GPSLogger::disable() { isLogging = false; }

void GPSLogger::enable() { isLogging = true; }

void GPSLogger::setLogFile(String name) { logFile = name; }

void GPSLogger::setInterval(unsigned int interval) { logInterval = interval; }

String GPSLogger::getLogPath() { return root + logFile; }

String GPSLogger::getIndexPath() { return root + INDEX_FILE_NAME; }

void GPSLogger::newLogFile(gps_fix fix) {
    logFile = getNbIndexEntries();

    LogIndexEntry entry = createLogIndexEntry(
                            getNbIndexEntries(),
                            logFile,
                            formatDate(fix) + F(" ") + formatTime(fix)
                        );

    addIndexEntry(entry);
    writeFile(logFile, ""); // Crée un fichier vide
}

void GPSLogger::log(gps_fix fix) {
    if (sdFailed) return;
    
    if (isLogging && (fix.valid.location || alwaysLog)) {
        if (logCounter == logInterval) {
            File file = SD.open(getLogPath(), FILE_WRITE);
            if (file) {
                writeCsvLine(file, fix);
            }

            file.close();

            logCounter = 1;
        } else {
            logCounter++;
        }
    }
}

void GPSLogger::writeFile(String filename, String line) {
    if (sdFailed) return;

    File file = SD.open(root + filename, FILE_WRITE);
    file.println(line);
    file.close();
}

void GPSLogger::writeCsvLine(File &file, gps_fix fix) {

    // DATE
    if (fix.valid.time)
        file.print(formatDate(fix));
    
    // TIME
    comma(file);
    if (fix.valid.time)
        file.print(formatTime(fix));

    // LATITUDE + LONGITUDE
    comma(file);
    if (fix.valid.location) {
        file.print(fix.latitude(), 7);
        file.print(F(","));
        file.print(fix.longitude(), 7);
    }

    // ALTITUDE (float-poing meters);
    comma(file);
    if (fix.valid.altitude)
        file.print(fix.altitude());

    // SPEED (km/h)
    comma(file);
    if (fix.valid.speed)
        file.print(fix.speed_kph());

    // HEADING (floating-point degrees)
    comma(file);
    if (fix.valid.heading)
        file.print(fix.heading());

    // SATELLITES COUNT
    comma(file);
    file.print(gps.sat_count);

    // SATELLITES INFO
    comma(file);
    if (fix.valid.satellites) {
        for (uint8_t i = 0; i < gps.sat_count; i++) {
            file.print(gps.satellites[i].id);
            file.print(' ');
            file.print(gps.satellites[i].elevation);
            file.print('/');
            file.print(gps.satellites[i].azimuth);
            file.print('@');
            if (gps.satellites[i].tracked)
                file.print(gps.satellites[i].snr);
            else
                file.print('-');
            
            if (i != gps.sat_count - 1)
                comma(file);
        }
    }

    file.println();
}

void GPSLogger::printIndexFile() { printFile(getIndexPath()); }

void GPSLogger::initIndexFile() {
    if (sdFailed) return;

    File index = SD.open(getIndexPath(), FILE_WRITE);
    index.close();
}

LogIndexEntry *GPSLogger::loadIndexFile() {
    if (sdFailed) return NULL;

    File index = SD.open(getIndexPath(), FILE_READ);

    LogIndexEntry *entries = new LogIndexEntry[getNbIndexEntries()];

    for (int i = 0; i < getNbIndexEntries(); i++) {
        int id = index.readStringUntil(',').toInt();
        String name = index.readStringUntil(',');
        String date = index.readStringUntil('\n');

        entries[i] = createLogIndexEntry(id, name, date);
    }

    index.close();
    return entries;
}

void GPSLogger::sendFile(String fileName, String date) {
    File file = SD.open(root + fileName, FILE_READ);
    Serial.flush();
    Serial.println(F("CMD_FS"));
    Serial.println(fileName);
    Serial.println(date);
    
    while (file.available() > 0) {
        Serial.write(file.read());
    }
    Serial.println(F("CMD_FS_"));

    file.close();
}