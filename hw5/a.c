#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER 5000

int main() {
    int fd;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    char rd[BUFFER], wr[BUFFER];
    while (1) {
        fd = open(myfifo, O_WRONLY);
        fgets(wr, BUFFER, stdin);
        write(fd, wr, strlen(wr) + 1);
        close(fd);

        fd = open(myfifo, O_RDONLY);
        read(fd, rd, sizeof(rd));
        printf("b: %s\n", rd);
        close(fd);
    }
    return 0;
}