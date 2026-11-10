// Small buffer-overflow demo for practising GDB.
// Goal: make check_login() return into win() instead of main().
#include <stdio.h>
#include <stdlib.h>

void win(void) {
    puts("You reached win()!");
    exit(0);
}

void check_login(void) {
    char name[32];
    puts("Username:");
    gets(name);  // intentionally unsafe
    printf("Hello, %s\n", name);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    check_login();
    puts("Login failed.");
    return 0;
}
