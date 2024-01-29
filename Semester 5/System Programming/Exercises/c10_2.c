#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd[2];
    if (pipe(fd) == 1) {
        printf("Could not pipe.");
        exit(1);
    }

    if (fork() == 0) {
        close(fd[0]);
        close(1);
        dup(fd[1]);

        if (execlp("who", "who", 0) != 0) {
            printf("Could not execlp.");
            exit(1);
        }
    }
    else {
        int status;
        wait(NULL);

        if (status != 0) {
            printf("Child bad.");
            exit(1);
        }

        close(fd[1]);
        close(0);
        dup(fd[0]);

        if (execlp("wc", "-c", 0) != 0) {
            printf("Could not execlp2.");
            exit(1);
        }
    }

    exit(0);
}