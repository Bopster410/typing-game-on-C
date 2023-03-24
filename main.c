#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define MAX_LEN 256

int main() {
    FILE *textFile = fopen("../texts/1.txt", "r");
    if (textFile == NULL) {
        puts("hui");
    }

    int len;
    fscanf(textFile, "%d\n", &len);
    char text[len + 1];
    fgets(text, len, textFile);
    fclose(textFile);

    // Get console handle
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    // Get console info
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(console, &consoleInfo);

    const int MAX_SYMBOLS_PER_ROW = consoleInfo.dwMaximumWindowSize.X - 5;

    // Initial console attributes
    WORD initialConsoleAttributes = consoleInfo.wAttributes;
    // Initial cursor info
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);

    COORD initialCoords = consoleInfo.dwCursorPosition;

    printf("%s", text);

    int rowsTotal = len / MAX_SYMBOLS_PER_ROW + len % MAX_SYMBOLS_PER_ROW != 0;
    COORD timerCoords = consoleInfo.dwCursorPosition;
    timerCoords.Y += rowsTotal + 1;

    SetConsoleCursorPosition(console, initialCoords);


    // Initial time
    clock_t prevTime = clock();
    int currentSymbolInd = 0;
    int seconds = 10;
    while (currentSymbolInd < len - 1 && seconds > 0){

        if (kbhit()) {
            char a = getch();
            if (text[currentSymbolInd] == a) {
                SetConsoleTextAttribute(console, FOREGROUND_GREEN);
            } else {
                SetConsoleTextAttribute(console, FOREGROUND_RED);
            }

            putchar(text[currentSymbolInd]);
            currentSymbolInd++;
        }

        GetConsoleScreenBufferInfo(console, &consoleInfo);
        COORD cursorCoords = consoleInfo.dwCursorPosition;

        SetConsoleTextAttribute(console, initialConsoleAttributes);
        SetConsoleCursorPosition(console, timerCoords);
        if ((clock() - prevTime) / CLOCKS_PER_SEC >= 1) {
            prevTime = clock();
            seconds--;
        }
        printf("%d seconds ", seconds);

        SetConsoleCursorPosition(console, cursorCoords);

    }
    timerCoords.Y += 2;
    SetConsoleCursorPosition(console, timerCoords);

    SetConsoleTextAttribute(console, initialConsoleAttributes);
    putchar('\n');

    system("PAUSE");
    return 0;
}
