#include <Arduino.h>
#include "command.h"
#include "globalState.h"
#include "logger.h"


// Sends to serial the content of INDEX file
void cmdList() {
    Serial.println(F(CMD_LS));
    views.context->logger.printIndexFile();
    Serial.println(F(CMD_LS_END));
}

void cmdDownload(char args[MAX_CMD_ARGS_LENGTH][MAX_CMD_ARGS], uint8_t nbArgs) {
    for (uint8_t i = 1; i < nbArgs; i++) {
        uint8_t id = atoi(args[i]);
        LogIndexEntry entry = views.context->logger.loadLogEntry(id);
        views.context->logger.sendFile(entry);
    }
}

void handleCommand(char* cmd, size_t length) {
    char args[MAX_CMD_ARGS_LENGTH][MAX_CMD_ARGS];
    uint8_t n = parseCommand(cmd, length, args);

    if (strcmp(args[0], CMD_LS) == 0) {
        cmdList();
    } else if (strcmp(args[0], CMD_DL) == 0 ) {
        cmdDownload(args, n);
    }
}

void cmdDownloadMinimal(int8_t id) {
    if (id >= views.context->logger.getNbIndexEntries()) {
        Serial.println(CMD_ERROR);
        return;
    }

    LogIndexEntry entry = views.context->logger.loadLogEntry(id);
    views.context->logger.sendFile(entry);
}

void handleCommandMinimal(int8_t cmd) {
    if (cmd >= 0) {
        cmdDownloadMinimal(cmd);
    } else if (cmd == -1) {
        cmdList();
    } else {
        Serial.println(CMD_ERROR);
    }
}

// Règle d'une instruction :
// <instruction>    ::= <cmd> { <args> }
// <cmd>           ::= DL | LS
// <args>           ::= alphanumerical
uint8_t parseCommand(char* buffer, size_t length, char args[MAX_CMD_ARGS_LENGTH][MAX_CMD_ARGS]) {
    char *tok = strtok(buffer, " ");
    
    uint8_t n = 0;
    while (tok != NULL && n < MAX_CMD_ARGS) {
        strcpy(args[n], tok);
        strtok(NULL, " ");
        n++;
    }

    return n;
}