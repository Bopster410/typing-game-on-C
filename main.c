#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "typgame.h"

int main(int argc, char *argv[]) {
    // Checks if input data is correct
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

    // If inputs is invalid, print help message and close the program
    if (status != SUCCESS) {
        const char *helpMessage = "\nUsage: RK1.exe [difficulty] [time] [errors]\n"
                                  "difficulty:\tone of 'easy', 'medium' or 'hard'\n"
                                  "time:\t\tmaximum time in seconds to complete the game\n"
                                  "errors:\t\tmaximum errors number\n";
        printf("%s", helpMessage);
        exit(status);
    }

    // Open file with text
    char *fileName = malloc(sizeof(char) * 32);
    // Random text file is chosen
    srand(time(NULL));
    sprintf(fileName, "texts/%s/%d.txt", argv[1], rand() % 3 + 1);
    FILE *textFile = fopen(fileName, "r");
    free(fileName);
    if (textFile == NULL) {
        puts("ERROR: something went wrong while opening the file!");
        exit(OPENING_FILE_ERROR);
    }

    // Reading data from file
    unsigned int len = 0;
    fscanf(textFile, "%d\n", &len);
    char *text = malloc(len + 1);
    fgets(text, len + 1, textFile);
    fclose(textFile);

    if (len == 0 || strlen(text) == 0) {
        puts("ERROR: wrong input file format");
        exit(FILE_FORMAT_ERROR);
    }

    // Get console handle
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get console info
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(console, &consoleInfo);

    // Initial console attributes
    WORD initialConsoleAttributes = consoleInfo.wAttributes;

    // Get initial cursor coords
    COORD initialCoords = consoleInfo.dwCursorPosition;

    // Initial cursor info
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);

    // Making cursor invisible
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);

    // Get max number of symbols per row and per page
    const int MAX_SYMBOLS_PER_ROW = consoleInfo.dwMaximumWindowSize.X;
    const int MAX_SYMBOLS_PER_PAGE = MAX_SYMBOLS_PER_ROW * 5;

    // Print first page of the text
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
    unsigned int currentSymbolInd = 0;
    unsigned int initialSeconds = atoi(argv[2]), seconds = initialSeconds,
        errorsLeft = atoi(argv[3]);

    // Set initial timer value
    updateTimer(&console, &timerCoords, seconds);

    // Main loop
    int correctSymbols = 0, wordsEntered = 0;
    while (currentSymbolInd < len && seconds > 0 && errorsLeft > 0) {
        // If keyboard button is pressed...
        if (kbhit()) {
            // ...get this button's character
            char a = getch(), currentSymbol = text[currentSymbolInd];

            // If it's the right one...
            if (currentSymbol == a) {
                // ...change current symbol's color to green...
                SetConsoleTextAttribute(console, BACKGROUND_GREEN);
                correctSymbols++;
            } else {
                // ...else change it to red
                SetConsoleTextAttribute(console, BACKGROUND_RED);
                errorsLeft--;
            }

            // If next symbol is a valid word delimiter, increase word's counter by 1
            char nextSym = text[currentSymbolInd + 1];
            if (isalpha(currentSymbol) && isWordsDelimiter(nextSym)) {
                wordsEntered++;
            }

            // Print current symbol with certain background
            // (depends on whether user's input is right or not)
            putchar(currentSymbol);

            // Increase current symbol's index
            currentSymbolInd++;

            // Set console attributes to default
            SetConsoleTextAttribute(console, initialConsoleAttributes);

            // If current page is over, print the next one
            if (currentSymbolInd % MAX_SYMBOLS_PER_PAGE == 0 && currentSymbolInd != 0) {
                SetConsoleCursorPosition(console, initialCoords);
                printf("%*c", MAX_SYMBOLS_PER_PAGE, ' ');
                SetConsoleCursorPosition(console, initialCoords);
                printf("%.*s", MAX_SYMBOLS_PER_PAGE, text + currentSymbolInd);
                SetConsoleCursorPosition(console, initialCoords);
            }
        }

        // If one whole second has gone...
        if ((clock() - prevTime) / CLOCKS_PER_SEC >= 1) {
            // Update number of seconds left
            prevTime = clock();
            seconds--;

            // Rewrite timer
            updateTimer(&console, &timerCoords, seconds);
        }
    }

    free(text);

    // Moving 2 rows down from timer position to print player's statistics
    timerCoords.Y += 2;
    SetConsoleCursorPosition(console, timerCoords);

    // Set default console attributes
    SetConsoleTextAttribute(console, initialConsoleAttributes);

    // Print the statistics
    float neededTimeMinutes = (initialSeconds - seconds) / 60.0;
    printf("correct symbols:\t%d\n", correctSymbols);
    printf("incorrect symbols:\t%d\n", currentSymbolInd - correctSymbols);
    printf("symbols total:\t\t%d\n", currentSymbolInd);
    printf("symbols per minute:\t%.2f\n", 1.0 * currentSymbolInd / neededTimeMinutes);
    printf("words per minute:\t%.2f\n", 1.0 * wordsEntered / neededTimeMinutes);

    // Making cursor visible
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(console, &cursorInfo);

    putchar('\n');

    system("PAUSE");
    return 0;
}
