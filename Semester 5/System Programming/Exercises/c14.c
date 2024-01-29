#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        exit(1);
    }

    if (fork() == 0) {
        int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (fd < 0) {
            exit(1);
        }

        int pr = 1;
        char c;
        while (read(0, &c, 1)) {
            if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {
                c = '*';
            }

            if (pr % 2 == 0) {
                write(fd, &c, 1);
            }
            
            if (c == '\n') {
                pr++;
            }
        }
        close(fd);
        exit(0);
    }
    else {
        int status;
        wait(&status);

        close(1);
        int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd2 < 0) {
            exit(0);
        }

        int fd = open(argv[1], O_RDONLY, 0644);
        if (fd < 0) {
            exit(0);
        }

        int cnt = 0;
        char c;
        while (read(fd, &c, 1)) {
            cnt++;
            if (c == '\n') {
                printf("%d\n", cnt);
                cnt = 0;
            }
        }
    }

    exit(0);
}
