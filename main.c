#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main() {
    printf("Hello, World!\r");
    
    char a;
    for (int i = 0; i < 10; i++) {
        a = getch();
        putchar(a);
    }
    putchar('\n');

    system("PAUSE");
    return 0;
}
