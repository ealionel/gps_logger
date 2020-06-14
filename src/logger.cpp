#include "logger.h"
#include "helper.h"
#include "globalState.h"
#include "command.h"
#include <EEPROM.h>

#define INDEX_FILE_NAME "INDEX"

LogIndexEntry createLogIndexEntry(uint8_t id, char date[9], char time[9]) {
    LogIndexEntry entry;
    entry.id = id;
    strcpy(entry.date, date);
    strcpy(entry.time, time);
    return entry;
}

void printLogIndexEntry(LogIndexEntry entry) {
    Serial.println("id\t" + String(entry.id, 10));
    Serial.print("name\t");
    Serial.println(entry.date);
}

GPSLogger::GPSLogger() {}

void GPSLogger::init(String root) {
    if (sdFailed) return;

    uint16_t eepromVal;

    // Initialisation du paramètre d'intervalle de log
    EEPROM.get(EEPROM_LOGINTERVAL_ADDR, eepromVal);
    if (eepromVal != 0)
        // Dans le cas où l'EEPROM ne serait pas initialisé (valeur 0)
        logInterval = eepromVal;

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
        logId = nbIndexEntries;
    } else {
        logId = nbIndexEntries - 1;
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

    index.print(entry.date);
    comma(index);

    index.print(entry.time);
    index.print('\n');

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
    // newLogFile((views.context)->fix);       // Recrée un nouveau fichier de log initial
}

void GPSLogger::disable() { isLogging = false; }

void GPSLogger::enable() { isLogging = true; }

void GPSLogger::setInterval(unsigned int interval) { logInterval = interval; }

String GPSLogger::getLogPath() { return root + logId; }

String GPSLogger::getIndexPath() { return root + INDEX_FILE_NAME; }

void GPSLogger::newLogFile(gps_fix &fix) {
    logId = getNbIndexEntries();

    char date[9];
    char time[9];

    formatDate(date, fix);
    formatTime(time, fix);

    LogIndexEntry entry = createLogIndexEntry(logId, date, time);

    addIndexEntry(entry);
    writeFile((String) logId, ""); // Crée un fichier vide
}

void GPSLogger::log(gps_fix fix) {
    if (sdFailed) return;
    
    if (isLogging && (fix.valid.location || alwaysLog)) {
        if (logIntervalCnt == logInterval) {
            File file = SD.open(getLogPath(), FILE_WRITE);
            if (file) {
                writeCsvLine(file, fix);
            }

            file.close();

            logIntervalCnt = 1;
        } else {
            logIntervalCnt++;
        }
    }
}

void GPSLogger::writeFile(String filename, String line) {
    if (sdFailed) return;

    File file = SD.open(root + filename, FILE_WRITE);
    file.print(line);
    file.close();
}

void GPSLogger::writeCsvLine(File &file, gps_fix &fix) {

    // DATE
    if (fix.valid.time) {
        char date[9];
        formatDate(date, fix);
        file.print(date);
    }
    
    // TIME
    comma(file);
    if (fix.valid.time) {
        char time[9];
        formatTime(time, fix);
        file.print(time);
    }

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

int GPSLogger::countLogs(uint8_t id) {
    File f = SD.open(root + id, FILE_READ);
    int n;
    while (f.available()) {
        if (f.read() == '\n') n++;
    }    
    f.close();
    return n - 1;
}

LogIndexEntry GPSLogger::parseLogEntry(uint8_t id) {
    File index = SD.open(getIndexPath(), FILE_READ);

    int n = 0;
    while (index.available() && n < id) {
        if (index.read() == '\n') n++;
    }

    char id_buffer[3]; // maximum 2 digits ID
    readBytesStringUntil(&index, ',', id_buffer, 3);

    char date[9]; // une date contient 8 caractères + null char
    readBytesStringUntil(&index, ',', date, 9);
    
    char time[9]; // 8 char + null char
    readBytesStringUntil(&index, '\n', time, 9);

    LogIndexEntry entry = createLogIndexEntry(
        atoi(id_buffer),
        date,
        time
    );

    index.close();

    return entry;
}

void GPSLogger::sendFile(LogIndexEntry entry) {

    File file = SD.open(root + entry.id, FILE_READ);

    Serial.println(F(CMD_DL));
    Serial.println(entry.id);
    Serial.println(entry.date);
    Serial.println(entry.time);
    
    while (file.available() > 0) {
        Serial.write(file.read());
    }
    Serial.println(F(CMD_DL_END));

    file.close();
}