#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    FILE *file;
    pid_t pid;

    file = fopen("wynik.txt", "w");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    fprintf(file, "START - wpis od procesu nadrzędnego\n");
    fflush(file);
    fclose(file);

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        file = fopen("wynik.txt", "a");
        if (!file) {
            perror("fopen");
            exit(1);
        }

        fprintf(file, "PRZETWARZANIE - wpis od procesu potomnego\n");
        fflush(file);
        fclose(file);

        exit(0);
    }

    wait(NULL);

    file = fopen("wynik.txt", "a");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    fprintf(file, "KONIEC - wpis od procesu nadrzędnego\n");
    fflush(file);
    fclose(file);

    file = fopen("wynik.txt", "r");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    char buffer[256];
    printf("\nZawartość pliku\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    fclose(file);
    return 0;
}