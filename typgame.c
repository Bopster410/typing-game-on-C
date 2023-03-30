#include "typgame.h"

bool isWordsDelimiter(char symbol) {
    const char *validDelimiters = ".,?!:;\")\n\t ";
    return strchr(validDelimiters, symbol) != NULL;
}

// Updates timer value
void updateTimer(const HANDLE *console, const COORD *timerCoords, unsigned int seconds) {
    // Get current console in formation...
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(*console, &consoleInfo);
    // ...and extract cursor's coords from it
    COORD cursorCoords = consoleInfo.dwCursorPosition;

    // Move cursor to timer position
    SetConsoleCursorPosition(*console, *timerCoords);

    // Print new time
    printf("%d seconds ", seconds);

    // Return cursor to it's initial coords
    SetConsoleCursorPosition(*console, cursorCoords);
}

// Validates input
statusCode_t checkInput(int argc, char *argv[]) {
    if (argc != 4) {
        return WRONG_ARGS_NUMBER;
    }

    char *difficulty = argv[1];
    if (strcmp(difficulty, "easy") != 0 && strcmp(difficulty, "medium") != 0 && strcmp(difficulty, "hard") != 0) {
        return WRONG_DIFFICULTY;
    }

    // inputVal - time in seconds
    int inputVal = atoi(argv[2]);
    if (inputVal <= 0 || inputVal > MAX_TIME_SECONDS) {
        return WRONG_TIME;
    }

    // inputVal - errors total
    inputVal = -1;
    sscanf(argv[3], "%d", &inputVal);
    if (inputVal < 0 || inputVal > MAX_ERRORS_COUNT) {
        return WRONG_ERRORS_COUNT;
    }

    return SUCCESS;
}
