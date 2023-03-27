#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

//#define DEBUG

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

int main() {
    FILE *textFile = fopen("../texts/2.txt", "r");
    if (textFile == NULL) {
        puts("hui");
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

            putchar(text[currentSymbolInd]);
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
