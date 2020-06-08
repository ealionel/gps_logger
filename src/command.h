#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <stdlib.h>

#define MAX_CMD_ARGS 5
#define MAX_CMD_ARGS_LENGTH 4

#define CMD_LS  "LS"
#define CMD_DL  "DL"

#define CMD_LS_END  "LS_"
#define CMD_DL_END  "DL_"

void cmdList();
void cmdDownload(char args[MAX_CMD_ARGS_LENGTH][MAX_CMD_ARGS], uint8_t nbArgs);

void handleCommand(char* cmd, size_t length);
uint8_t parseCommand(char* buffer, size_t length, char args[MAX_CMD_ARGS_LENGTH][MAX_CMD_ARGS]);

#endif