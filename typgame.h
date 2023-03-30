#ifndef RK1_TYPGAME_H
#define RK1_TYPGAME_H

#define MAX_TIME_SECONDS 500
#define MAX_ERRORS_COUNT 50

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <search.h>
#include <stdio.h>

typedef enum statusCode {
    SUCCESS,
    WRONG_ARGS_NUMBER,
    WRONG_DIFFICULTY,
    WRONG_TIME,
    WRONG_ERRORS_COUNT,
    OPENING_FILE_ERROR,
    FILE_FORMAT_ERROR
} statusCode_t;

// Checks if current symbols is a valid words delimiter
bool isWordsDelimiter(char symbol);

// Updates timer value
void updateTimer(const HANDLE *console, const COORD *timerCoords, unsigned int seconds);

// Validates input
statusCode_t checkInput(int argc, char *argv[]);

#endif //RK1_TYPGAME_H
