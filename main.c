#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

int main() {
    char text[256] = "Hello, world!";
    printf("%s\r", text);

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO initialConsoleInfo;
    GetConsoleScreenBufferInfo(console, &initialConsoleInfo);
    WORD initialConsoleAttributes = initialConsoleInfo.wAttributes;

    char a;
    for (int i = 0; i < 10; i++) {
        a = getch();
        if (text[i] == a) {
            SetConsoleTextAttribute(console, FOREGROUND_GREEN);
        } else {
            SetConsoleTextAttribute(console, FOREGROUND_RED);
        }

        putchar(text[i]);
    }
    SetConsoleTextAttribute(console, initialConsoleAttributes);
    putchar('\n');

    system("PAUSE");
    return 0;
}
