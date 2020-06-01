#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <NMEAGPS.h>

typedef struct LogIndexEntry {
    int id;
    String fileName;
    // int nbLogs; // Nombre de points enregistrés
    String date;
} LogIndexEntry;

LogIndexEntry createLogIndexEntry(int id, String fileName, String date);
void printLogIndexEntry(LogIndexEntry entry);

class GPSLogger {
    public:
        bool isLogging = false;
        unsigned int logInterval = 1;
        
        GPSLogger();

        void init(String root = "/LOGS/");

        void log(gps_fix fix);
        void disable();
        void enable();
        void setLogFile(String name);
        void setInterval(unsigned int interval);

        LogIndexEntry* loadIndexFile();

        // Compte le nombre d'entrées dans le fichier index
        int countIndexEntries();

        // Retourne la valeur de indexEntries (ne recompte pas)
        int getNbIndexEntries();

        void addIndexEntry(LogIndexEntry entry);

        // Si le fichier n'existe pas, crée juste le fichier vide
        // Sinon ne fait rien
        void initIndexFile();

        // Removes all files in directory and resets index file
        void clearDirectory();
        
        // Retourne le chemin absolu du fichier en cours de log
        String getLogPath();

        // Retourne le chemin absolu du fichier index de ce repertoire
        String getIndexPath();

        void writeCsvLine(File &file, gps_fix fix);
        void writeFile(String filename, String line);

        void newLogFile();

        int logId();

        void printIndexFile();
    private:
        String logFile;
        String root;

        int nbIndexEntries;
};

#endif