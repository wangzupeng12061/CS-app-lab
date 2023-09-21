#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 32

int main(int argc, char *argv[]) {
    int fd, ret;
    char buf[MAX_BUFFER_SIZE];
    int read_size;

    if (argc < 3) {
        printf("Usage: %s <r> <read_size>\n", argv[0]);
        return -1;
    }

    fd = open("/dev/kfifo_buffer", O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return fd;
    }

    if (strcmp(argv[1], "r") != 0) {
        printf("Invalid mode: %s\n", argv[1]);
        close(fd);
        return -1;
    }

    read_size = atoi(argv[2]);
    if (read_size <= 0 || read_size > MAX_BUFFER_SIZE) {
        printf("Invalid read size: %s\n", argv[2]);
        close(fd);
        return -1;
    }

    memset(buf, 0, sizeof(buf));

    int total_read = 0;
    while (total_read < read_size) {
        ret = read(fd, buf + total_read, read_size - total_read);
        if (ret < 0) {
            perror("Failed to read from the device");
            close(fd);
            return ret;
        } else if (ret == 0) {
            printf("Reached end of file\n");
            break;
        }

        total_read += ret;
    }

    if (total_read >= read_size) {
        printf("Read %d bytes from the device: %s\n", total_read, buf);
    }

    close(fd);
    return 0;
}

