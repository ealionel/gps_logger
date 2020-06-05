#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <NMEAGPS.h>

typedef struct LogIndexEntry {
    uint8_t id;
    String fileName;
    // int nbLogs; // Nombre de points enregistrés
    String date;
} LogIndexEntry;

LogIndexEntry createLogIndexEntry(uint8_t id, String fileName, String date);
void printLogIndexEntry(LogIndexEntry entry);

class GPSLogger {
    public:
        bool sdFailed = false;
        bool isLogging = false;
        bool alwaysLog = true;
        int logInterval = 5;
        
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

        void newLogFile(gps_fix fix);

        // Envoie le contenue de file (dans le dossier root) sur le port série
        // en suivant un protocole de communication custom
        void sendFile(String file, String date);

        void printIndexFile();
        String root;
    private:
        String logFile;

        int nbIndexEntries;
        int logCounter = 1; // Compteur pour l'intervalle d'enregistrement
};

#endif