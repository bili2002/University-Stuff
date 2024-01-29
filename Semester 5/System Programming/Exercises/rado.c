#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    int fd[2];
    int n;
    char buf[10];
    pipe(fd);

    if (fork() == 0) {
        close(fd[0]);
        write(fd[1], "hello\n", 6);
        exit(0);
    }
    else {
        wait(NULL);
        close(fd[1]);
        while(n=read(fd[0], buf, 10) != 0) {
            write(1, buf, n);
        }
    
    }
    exit(0);
}