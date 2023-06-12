#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MSG_SIZE 64

int main (int argc, char **argv) {
    int fd, fdw;
    char buffer[MSG_SIZE + 1];
    ssize_t read_bytes;

    if (argc != 3) {
        fprintf(stderr,"Usage:  %s <READ_FILE> <WRITE_FILE>\n", argv[0]);
        exit(1);
    }

    char *read_file = argv[1];
    char *write_file = argv[2];

    if ((fd = open(read_file, O_RDONLY)) < 0){
        printf("Can\'t open file %s\n", read_file);
        exit(-1);
    }
    if ((fdw = open(write_file, O_WRONLY)) < 0){
        printf("Can\'t open file %s\n", write_file);
        exit(-1);
    }
    do {
        memset(&buffer, 0, sizeof(buffer));
        read_bytes = read(fd, buffer, MSG_SIZE);
        if (read_bytes == -1) {
            printf("Can\'t read this file\n");
            exit(-1);
        }
        buffer[read_bytes] = '\0';
        

        if (write(fdw, buffer, read_bytes) < 0) {
            printf("Can\'t write this file\n");
            exit(-1);
        }
    } while (read_bytes == MSG_SIZE);

    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }
    if (close(fdw) < 0) {
        printf("Can\'t close file\n");
    }
    return 0;
}