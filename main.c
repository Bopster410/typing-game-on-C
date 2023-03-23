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
    char text[len];
    fgets(text, len, textFile);
    fclose(textFile);

    printf("%s\r", text);

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
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);
    COORD c1 = {0, 2};

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
        COORD c = consoleInfo.dwCursorPosition;

        SetConsoleTextAttribute(console, initialConsoleAttributes);
        SetConsoleCursorPosition(console, c1);
        if ((clock() - prevTime) / CLOCKS_PER_SEC >= 1) {
            prevTime = clock();
            seconds--;
        }
        printf("%d seconds ", seconds);

        SetConsoleCursorPosition(console, c);

    }

    SetConsoleTextAttribute(console, initialConsoleAttributes);
    putchar('\n');

    system("PAUSE");
    return 0;
}
