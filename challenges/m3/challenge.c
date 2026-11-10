#define _GNU_SOURCE

#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

__attribute__((noinline))
void status(void) {
    puts("Helpdesk status: all systems operational.");
}

__attribute__((noinline))
void issue_secret_key(void) {
    char username[10];
    uint64_t username_value = 0;
    uint64_t secret_key;
    size_t index;

    printf("Eight-character username: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        _exit(0);
    }
    username[strcspn(username, "\n")] = '\0';
    if (strlen(username) != 8) {
        puts("Username must contain exactly eight characters.");
        return;
    }

    for (index = 0; index < 8; index++) {
        username_value |= (uint64_t)(unsigned char)username[index] << (8 * index);
    }

    secret_key = (uint64_t)(uintptr_t)status ^ username_value;
    printf("Secret key: %016" PRIx64 "\n", secret_key);
}

__attribute__((noinline, force_align_arg_pointer))
void win(void) {
    char flag[128];
    const char *path;
    FILE *file;

    path = getenv("FLAG_PATH");
    if (path == NULL) {
        path = "/app/flag.txt";
    }

    file = fopen(path, "r");
    if (file == NULL || fgets(flag, sizeof(flag), file) == NULL) {
        puts("Helpdesk: could not read the flag.");
        _exit(1);
    }

    if (file != NULL) {
        fclose(file);
    }
    printf("Supervisor console unlocked.\nFlag: %s", flag);
    _exit(0);
}

__attribute__((noinline))
void store_request(const char *request, ssize_t length) {
    int file;

    if (length <= 0) {
        _exit(0);
    }

    file = open("/tmp/helpdesk-requests.log", O_WRONLY | O_CREAT | O_APPEND, 0600);
    if (file < 0) {
        puts("Helpdesk: could not store the request.");
        return;
    }

    if (write(file, request, (size_t)length) != length) {
        puts("Helpdesk: could not store the complete request.");
    }
    close(file);
}

__attribute__((noinline))
void take_request(void) {
    char request[64];

    puts("Describe your request:");
    store_request(request, read(STDIN_FILENO, request, 256));
    puts("Your request has been stored.");
}

int main(void) {
    char choice[16];

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    alarm(30);

    puts("Campus IT Helpdesk v2");
    while (1) {
        puts("\n1. Issue user secret key");
        puts("2. Open a helpdesk ticket");
        puts("3. Quit");
        printf("Choice: ");

        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            return 0;
        }

        if (strcmp(choice, "1\n") == 0) {
            issue_secret_key();
        } else if (strcmp(choice, "2\n") == 0) {
            take_request();
            return 0;
        } else if (strcmp(choice, "3\n") == 0) {
            return 0;
        } else {
            puts("Unknown option.");
        }
    }
}
