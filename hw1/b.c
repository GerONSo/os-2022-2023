#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FIFO  "tmp.input"
#define OUTPUT_FIFO "tmp.output"
#define BUFFER_SIZE 5000

char* itoa(int val) {
    if (val == 0) {
        return "0";
    }
	static char buf[32] = {0};
	int i = 30;
	for(; val && i; --i, val /= 10)
		buf[i] = "0123456789"[val % 10];
	
	return &buf[i + 1];
	
}

char is_digit(char c) {
    char* digits = "0123456789";
    for (int i = 0; i < 10; ++i) {
        if (digits[i] == c) {
            return 1;
        }
    }
    return 0;
}

int count(char* string, int size) {
    int result = 0;

	for (int i = 0; i < size; ++i) {
        if (i < size && is_digit(string[i])) {
            result++;
        }
        while (i < size && is_digit(string[i])) {
            i++;
        }
    }
    return result;
}

void write_to_fifo(const char input_string[], int size) {
    int fifo;

    if ((fifo = open(OUTPUT_FIFO, O_WRONLY)) < 0) {
        printf("[error] Can\'t open FIFO for writing\n");
        exit(-1);
    }

    write(fifo, input_string, size);

    if (close(fifo) < 0) {
        printf("[error] Can\'t close FIFO\n"); 
        exit(-1);
    }
}

int main() {
    int     fd;
    size_t  size;
    char    resstring[BUFFER_SIZE];
    char    name[] = "tmp.input";

    (void)umask(0);

    if ((fd = open(name, O_RDWR)) < 0) {
        printf("[error] Can\'t open FIFO for reading\n");
        exit(-1);
    }

    size = read(fd, resstring, BUFFER_SIZE);

    char* result = itoa(count(resstring, size));

    printf("%s\n", result);

    write_to_fifo(result, strlen(result));

    if (close(fd) < 0) {
        printf("[error] Can\'t close FIFO\n"); 
        exit(-1);
    }

    return 0;
}