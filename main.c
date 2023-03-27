#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define MAX_LEN 256

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
    FILE *textFile = fopen("../texts/1.txt", "r");
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

    // Get max number of symbols per row
    const int MAX_SYMBOLS_PER_ROW = consoleInfo.dwMaximumWindowSize.X - 5;

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

    // Print text
    printf("%s", text);

    // Find proper timer coords
    int rowsTotal = len / MAX_SYMBOLS_PER_ROW + len % MAX_SYMBOLS_PER_ROW != 0;
    COORD timerCoords = consoleInfo.dwCursorPosition;
    timerCoords.Y += rowsTotal + 1;

    // Return cursor to initial coords
    SetConsoleCursorPosition(console, initialCoords);

    // Initial time
    clock_t prevTime = clock();
    int currentSymbolInd = 0;
    int initialSeconds = 10, seconds = initialSeconds;

    // Set initial timer value
    updateTimer(&console, &timerCoords, seconds);

    // Main loop
    int correctSymbols = 0, symbolsEnteredTotal = 0, wordsEntered = 0;
    while (currentSymbolInd < len - 1 && seconds > 0) {
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
            symbolsEnteredTotal++;

            char nextSym = text[currentSymbolInd + 1];
            if (isalpha(a) && (nextSym == ' ' || nextSym == ',' || nextSym == '.')) {
                wordsEntered++;
            }

            putchar(text[currentSymbolInd]);
            currentSymbolInd++;
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

    float neededTimeMinutes = (initialSeconds - seconds) / 60.0;
    printf("correct symbols: %d\n", correctSymbols);
    printf("incorrect symbols: %d\n", symbolsEnteredTotal - correctSymbols);
    printf("symbols total: %d\n", symbolsEnteredTotal);
    printf("symbols per minute: %.2f\n", 1.0 * symbolsEnteredTotal / neededTimeMinutes);
    printf("words per minute: %.2f\n", 1.0 * wordsEntered / neededTimeMinutes);

    // Making cursor visible
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(console, &cursorInfo);

    SetConsoleTextAttribute(console, initialConsoleAttributes);
    putchar('\n');

    system("PAUSE");
    return 0;
}
