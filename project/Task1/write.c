#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 32

int main(int argc, char *argv[]){
    int fd, ret;
    char buf[MAX_BUFFER_SIZE];

    if (argc < 2) {
        printf("Usage: %s <w> [<data>]\n", argv[0]);
        return -1;
    }

    fd = open("/dev/kfifo_buffer", O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return fd;
    }

    if (strcmp(argv[1], "w") == 0) {
        if (argc < 3) {
            printf("Usage: %s w <data>\n", argv[0]);
            return -1;
        }
        ret = write(fd, argv[2], strlen(argv[2]));
        if (ret < 0) {
            perror("Failed to write to the device");
            return ret;
        }
        printf("Wrote %d bytes to the device\n", ret);
    }  else {
        printf("Invalid mode: %s\n", argv[1]);
        return -1;
    }

    close(fd);
    return 0;
}

