#include "logger.h"

#include "helper.h"

#define INDEX_FILE_NAME "INDEX"

LogIndexEntry createLogIndexEntry(int id, String fileName, String date) {
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
        newLogFile();
    } else {
        logFile = String(getNbIndexEntries() - 1, 10) + ".CSV";
    }
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

    index.print(entry.id);
    index.print(F(","));
    index.print(entry.fileName);
    index.print(F(","));
    index.print(entry.date);
    index.print(F("\n"));

    index.close();

    nbIndexEntries++;
}

void GPSLogger::clearDirectory() {
    File dir = SD.open(root);

    Serial.println(root);

    File entry;
    while (true) {
        entry = dir.openNextFile();

        if (!entry) {
            // Serial.println("Done");
            break;
        }

        String fileName = root + entry.name();

        if (SD.remove(fileName)) {
            // Serial.print("Removed " + fileName);
            // Serial.print(F("\n"));
        } else {
            // Serial.print("Coudln't remove " + fileName);
            // Serial.print(F("\n"));
            break;
        }
        entry.close();
    }

    dir.close();

    // Refait plus ou moins comme init() :
    initIndexFile();     // Recrée le fichier index
    nbIndexEntries = 0;  // Remet le nombre d'entrées à 0
    newLogFile();        // Recrée un nouveau fichier de log initial
}

void GPSLogger::disable() { isLogging = false; }

void GPSLogger::enable() { isLogging = true; }

void GPSLogger::setLogFile(String name) { logFile = name; }

void GPSLogger::setInterval(unsigned int interval) { logInterval = interval; }

String GPSLogger::getLogPath() { return root + logFile; }

String GPSLogger::getIndexPath() { return root + INDEX_FILE_NAME; }

void GPSLogger::newLogFile() {
    logFile = getNbIndexEntries() + ".CSV";
    LogIndexEntry entry =
        createLogIndexEntry(getNbIndexEntries(), logFile, "2020");
    addIndexEntry(entry);
    writeFile(logFile, "");  // Crée un fichier vide
}

void GPSLogger::log(gps_fix fix) {
    if (isLogging) {
        if (logCounter == logInterval) {
            File file = SD.open(getLogPath(), FILE_WRITE);
            if (file) {
                // Serial.println("Logging !");
                writeCsvLine(file, fix);
            } else {
                // Serial.print("LOGGER : Failed to open ");
                Serial.println(logFile);
            }
            file.close();

            logCounter = 1;
        } else {
            logCounter++;
        }
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

    file.print(F("\n"));
}

void GPSLogger::printIndexFile() { printFile(getIndexPath()); }

void GPSLogger::initIndexFile() {
    File index = SD.open(getIndexPath(), FILE_WRITE);
    index.close();
    // Serial.print("Initializing ");
    Serial.println(getIndexPath());
}

LogIndexEntry *GPSLogger::loadIndexFile() {
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