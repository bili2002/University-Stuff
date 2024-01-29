#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    int status1, status2, status3;
    int pr1, pr2, pr3;

    if (pr1 = fork() == 0) {
        close(1);
        int fd = open("/home/KN/admin/active_users", O_WRONLY);
        if (fd < 0) {
            printf("No fd");
            exit(1);
        }

        if (execlp("who", "who", "-u", 0) != 0) {
            printf("Bad process 1");
            exit(1);
        }
    }
    else  {
        wait(&status1);

        if (pr2 = fork() == 0) {
            close(0);
            int fd = open(argv[1], O_RDONLY);

            if (execlp("tail", "tail -6l", 0) != 0) {
                printf("Bad process 2");
                exit(1);
            } 
        }
        else {
            wait(&status2);

            if (pr3 = fork() == 0) {
                if (execvp(argv[2], argv + 2)) {
                    printf("Bad process 3");
                    exit(1);
                } 
            }
            else {
                wait(&status3);

                if (status3 == 0) {
                    printf("S1:%d\nS2:%d\nS3:%d\n", status1, status2, status3);
                }
                else {
                    printf("P1:%d\nP2:%d\nP3:%d\n", pr1, pr2, pr3);
                }
            }
        }
    }
}