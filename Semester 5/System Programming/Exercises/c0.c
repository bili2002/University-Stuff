#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        return -1;
    }

    char* file1 = argv[1];
    char* file2 = argv[2];

    int fd = open(file1, O_RDONLY);
    if (fd == -1) {
        printf("Didn't open");
        exit(-1);
    }

    int fd2 = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0777);

    const int MAX_BUFF = 10;
    int buff_size;
    char buff[MAX_BUFF];

    do {
        buff_size = read(fd, buff, MAX_BUFF);
        
        if (buff_size < 0) {     
            printf("Didn't open");
            exit(-1);
        }

        write(fd2, buff, buff_size);
    }
    while (buff_size > 0);

    close(fd);
    close(fd2);

    return 0;
}

