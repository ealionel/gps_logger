#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <NMEAGPS.h>

typedef struct LogIndexEntry {
    uint8_t id;
    // int nbLogs; // Nombre de points enregistrés
    char date[9];
    char time[9];
} LogIndexEntry;

LogIndexEntry createLogIndexEntry(uint8_t id, char date[9], char time[9]);
void printLogIndexEntry(LogIndexEntry entry);

class GPSLogger {
    public:
        bool sdFailed = false;
        bool isLogging = false;
        bool alwaysLog = false;
        uint16_t logInterval = 5;
        
        GPSLogger();

        void init(String root = "/LOGS/");
        void log(gps_fix fix);
        void disable();
        void enable();
        void setInterval(unsigned int interval);

        LogIndexEntry loadLogEntry(uint8_t id);

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

        void writeCsvLine(File &file, gps_fix &fix);

        void writeFile(String filename, String line);

        // Retourne le nombre de lignes dans le fichier de log avec l'id donné
        // (retire le trailing \n donc retourne n-1)
        int countLogs(uint8_t id);

        void newLogFile(gps_fix &fix);

        // Envoie le contenue de file (dans le dossier root) sur le port série
        // en suivant un protocole de communication custom
        void sendFile(LogIndexEntry entry);

        void printIndexFile();
        String root;

        // ID du fichier de log actuel
        uint8_t logId;

        // Nombre de points enregistrés dans le log actuel
        uint16_t logsCount = 0;
    private:

        int nbIndexEntries;
        uint16_t logIntervalCnt = 1; // Compteur pour l'intervalle d'enregistrement
};

#endif