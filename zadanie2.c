#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Użycie: %s <liczba_procesow>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Liczba procesów musi być > 0\n");
        return 1;
    }

    pid_t pid;

    for (int i = 0; i < n; i++) {
        pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            printf("Potomek %d: PID=%d, PPID=%d\n",
                   i + 1, getpid(), getppid());

            exit(i + 1);
        }
    }

    for (int i = 0; i < n; i++) {
        int status;
        pid_t child_pid = wait(&status);

        if (child_pid > 0) {
            if (WIFEXITED(status)) {
                printf("Proces PID=%d zakończył się kodem %d\n",
                       child_pid, WEXITSTATUS(status));
            } else {
                printf("Proces PID=%d zakończył się nieprawidłowo\n",
                       child_pid);
            }
        }
    }

    return 0;
}