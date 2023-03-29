#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

//#define DEBUG
#define MAX_TIME_SECONDS 500
#define MAX_ERRORS_COUNT 50

typedef enum statusCode {
    SUCCESS,
    WRONG_ARGS_NUMBER,
    WRONG_DIFFICULTY,
    WRONG_TIME,
    WRONG_ERRORS_COUNT
} statusCode_t;


// Updates timer value
void updateTimer(const HANDLE *console, const COORD *timerCoords, int seconds) {
    // Get current console information...
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

int main(int argc, char *argv[]) {
    statusCode_t status = checkInput(argc, argv);
    switch(status)
    {
        case WRONG_ARGS_NUMBER:
            puts("ERROR: wrong number of arguments was entered!");
            break;
        case WRONG_DIFFICULTY:
            puts("ERROR: wrong difficulty was chosen! Please, enter 'easy', 'medium' or 'hard'.");
            break;
        case WRONG_TIME:
            printf("ERROR: wrong time was entered! Valid time in seconds is between 0 and %d\n", MAX_TIME_SECONDS + 1);
            break;
        case WRONG_ERRORS_COUNT:
            printf("ERROR: wrong errors count was entered! Valid errors number is from 0 to %d\n", MAX_ERRORS_COUNT);
            break;
    }

    if (status != SUCCESS) {
        const char *helpMessage = "\nUsage: RK1.exe [difficulty] [time] [errors]\n"
                                  "difficulty:\tone of 'easy', 'medium' or 'hard'\n"
                                  "time:\t\tmaximum time in seconds to complete the game\n"
                                  "errors:\t\tmaximum errors number\n";
        printf("%s", helpMessage);
        return status;
    }


    FILE *textFile = fopen("../texts/2.txt", "r");
    if (textFile == NULL) {
        puts("hui");
        return 1;
    }

    // Reading data from file
    int len;
    fscanf(textFile, "%d\n", &len);
    char text[len + 1];
    fgets(text, len + 1, textFile);
    fclose(textFile);

    // Get console handle
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get console info
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(console, &consoleInfo);

    // Initial console attributes
    WORD initialConsoleAttributes = consoleInfo.wAttributes;

    // Initial cursor info
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);

    // Making cursor invisible
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);

    // Get initial cursor coords
    COORD initialCoords = consoleInfo.dwCursorPosition;

    // Get max number of symbols per row and per page
    const int MAX_SYMBOLS_PER_ROW = consoleInfo.dwMaximumWindowSize.X;
    const int MAX_SYMBOLS_PER_PAGE = MAX_SYMBOLS_PER_ROW * 5;

    // Print text
    printf("%.*s", MAX_SYMBOLS_PER_PAGE, text);


    // Update console info
    GetConsoleScreenBufferInfo(console, &consoleInfo);

    // Find proper timer coords
    int rowsTotal = len / MAX_SYMBOLS_PER_ROW + len % MAX_SYMBOLS_PER_ROW != 0;
    COORD timerCoords = consoleInfo.dwCursorPosition;
    timerCoords.Y += rowsTotal + 1;
    timerCoords.X = 0;

    // Return cursor to initial coords
    SetConsoleCursorPosition(console, initialCoords);

    // Initial time
    clock_t prevTime = clock();
    int currentSymbolInd = 0;
    int initialSeconds = 500, seconds = initialSeconds;

    // Set initial timer value
    updateTimer(&console, &timerCoords, seconds);

    // Main loop
    int numOfSymbolsToPrint = MAX_SYMBOLS_PER_PAGE;
    int correctSymbols = 0, wordsEntered = 0;
    while (currentSymbolInd < len && seconds > 0) {
        // If keyboard button is pressed...
        if (kbhit()) {
            // ...get this button's character
            char a = getch();

            // If it's the right one...
            if (text[currentSymbolInd] == a) {
                // ...change current symbol's color to green...
                SetConsoleTextAttribute(console, FOREGROUND_GREEN);
                correctSymbols++;
            } else {
                // ...else change it to red
                SetConsoleTextAttribute(console, FOREGROUND_RED);
            }

            char nextSym = text[currentSymbolInd + 1];
            if (isalpha(a) && (nextSym == ' ' || nextSym == ',' || nextSym == '.')) {
                wordsEntered++;
            }

            if (text[currentSymbolInd] == ' ') {
                putchar('_');
            } else {
                putchar(text[currentSymbolInd]);
            }
            currentSymbolInd++;

            if (currentSymbolInd % numOfSymbolsToPrint == 0 && currentSymbolInd != 0) {
                SetConsoleTextAttribute(console, initialConsoleAttributes);
#ifdef DEBUG
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                GetConsoleScreenBufferInfo(console, &csbi);
#endif
                SetConsoleCursorPosition(console, initialCoords);
#ifdef DEBUG
                GetConsoleScreenBufferInfo(console, &csbi);
#endif
                printf("%*c", numOfSymbolsToPrint, ' ');
#ifdef DEBUG
                GetConsoleScreenBufferInfo(console, &csbi);
#endif
                SetConsoleCursorPosition(console, initialCoords);
#ifdef DEBUG
                GetConsoleScreenBufferInfo(console, &csbi);
#endif
                printf("%.*s", numOfSymbolsToPrint, text + currentSymbolInd);
#ifdef DEBUG
                GetConsoleScreenBufferInfo(console, &csbi);
#endif
                SetConsoleCursorPosition(console, initialCoords);
#ifdef DEBUG
                GetConsoleScreenBufferInfo(console, &csbi);
#endif

            }
        }

        // If one whole second has gone...
        if ((clock() - prevTime) / CLOCKS_PER_SEC >= 1) {
            // Set console attributes to default
            SetConsoleTextAttribute(console, initialConsoleAttributes);

            // Update number of seconds left
            prevTime = clock();
            seconds--;

            // Rewrite timer
            updateTimer(&console, &timerCoords, seconds);
        }



    }
    timerCoords.Y += 2;
    SetConsoleCursorPosition(console, timerCoords);

    SetConsoleTextAttribute(console, initialConsoleAttributes);

    float neededTimeMinutes = (initialSeconds - seconds) / 60.0;
    printf("correct symbols: %d\n", correctSymbols);
    printf("incorrect symbols: %d\n", currentSymbolInd - correctSymbols);
    printf("symbols total: %d\n", currentSymbolInd);
    printf("symbols per minute: %.2f\n", 1.0 * currentSymbolInd / neededTimeMinutes);
    printf("words per minute: %.2f\n", 1.0 * wordsEntered / neededTimeMinutes);

    // Making cursor visible
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(console, &cursorInfo);

    putchar('\n');

    system("PAUSE");
    return 0;
}
