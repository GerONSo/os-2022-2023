#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 5000
#define INPUT_FIFO  "tmp.input"
#define OUTPUT_FIFO "tmp.output"

void write_to_fifo(const char input_string[], int size) {
    int fifo;

    if ((fifo = open(INPUT_FIFO, O_WRONLY)) < 0) {
        perror("[error] Can\'t open FIFO for writing\n");
        exit(-1);
    }

    write(fifo, input_string, size);

    if (close(fifo) < 0) {
        perror("[error] Can\'t close FIFO\n"); 
        exit(-1);
    }
}

void write_input(char input_name[]) {
    int    fd;
    char   input_string[BUFFER_SIZE];
    size_t size;

    if ((fd = open(input_name, O_RDONLY)) < 0) {
        perror("[error] Can\'t open input for reading\n");
        exit(-1);
    }

    size = read(fd, input_string, BUFFER_SIZE);

    if (size < 0) {
        perror("[error] Can\'t read string from output\n");
        exit(-1);
    }

    if (close(fd) < 0) {
        perror("[error] Can\'t close input\n"); 
        exit(-1);
    }

    write_to_fifo(input_string, size);
}

void write_output(char output_name[], char result[], int size) {
    int output;

    if ((output = open(output_name, O_WRONLY)) < 0) {
        printf("[error] Can\'t open output for writing\n");
        exit(-1);
    } 

    write(output, result, size);

    if (close(output) < 0) {
        printf("[error] Can\'t close output\n"); 
        exit(-1);
    }
}

void read_result(char output_name[]) {
    int  fifo_output;
    int  size;
    char result[BUFFER_SIZE];
    if ((fifo_output = open(OUTPUT_FIFO, O_RDONLY)) < 0) {
        printf("[error] Can\'t open FIFO for reading\n");
        exit(-1);
    }

    size = read(fifo_output, result, BUFFER_SIZE);

    write_output(output_name, result, size);

    if (close(fifo_output) < 0) {
        printf("[error] Can\'t close FIFO\n"); 
        exit(-1);
    }
    
}

int main(int argc, char **argv) {
    (void)umask(0);

    char *input_file = "";
    char *output_file = "";

    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-i") == 0) {
            input_file = argv[i + 1];
        } else if (strcmp(argv[i], "-o") == 0) {
            output_file = argv[i + 1];
        } else {
            printf("[error] Incorrect argument: %s\n", argv[i]);
            exit(-1);
        }
    }

    if (strcmp(input_file, "") == 0) {
        printf("Add -i ${input_file_name}\n");
        exit(-1);
    }

    if (strcmp(output_file, "") == 0) {
        printf("Add -o ${output_file_name}\n");
        exit(-1);
    }

    if (mkfifo(INPUT_FIFO, 0666)) {
        printf("[error] Can\'t create INPUT_FIFO\n");
        exit(-1);
    }
    if (mkfifo(OUTPUT_FIFO, 0666)) {
        perror("[error] Can\'t create OUTPUT_FIFO\n");
        exit(-1);
    }

    write_input(input_file);
    read_result(output_file);

    return 0;
}