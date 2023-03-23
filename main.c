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

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO initialConsoleInfo;
    GetConsoleScreenBufferInfo(console, &initialConsoleInfo);
    WORD initialConsoleAttributes = initialConsoleInfo.wAttributes;

    clock_t prevTime = clock();
    for (int i = 0; i < len - 1; i++) {
        char a = getch();
        if (text[i] == a) {
            SetConsoleTextAttribute(console, FOREGROUND_GREEN);
        } else {
            SetConsoleTextAttribute(console, FOREGROUND_RED);
        }

        putchar(text[i]);
    }
    int seconds = (clock() - prevTime) / CLOCKS_PER_SEC;

    SetConsoleTextAttribute(console, initialConsoleAttributes);
    putchar('\n');
    printf("%d seconds\n", seconds);

    system("PAUSE");
    return 0;
}
