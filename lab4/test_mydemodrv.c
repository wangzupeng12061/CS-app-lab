#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_NAME "/dev/mydemodrv"
#define MYDEMODRV_ADD    0x01
#define MYDEMODRV_SUB    0x02
#define MYDEMODRV_MAX    0x03

int main()
{
    int fd;
    int val1 = 3;
    int val2 = 4;
    int result = 0;

    // Open the device
    fd = open(DEVICE_NAME, O_RDWR | O_CREAT);
    if (fd < 0) {
        printf("Failed to open device %s\n", DEVICE_NAME);
        return -1;
    }

    // Write the two integer values to the device
    if (write(fd, &val1, sizeof(int)) < 0) {
        printf("Failed to write val1 to device\n");
        close(fd);
        return -1;
    }

    if (write(fd, &val2, sizeof(int)) < 0) {
        printf("Failed to write val2 to device\n");
        close(fd);
        return -1;
    }

    // Add the two values
    if (ioctl(fd, MYDEMODRV_ADD, &result) < 0) {
        printf("Failed to perform add operation\n");
        close(fd);
        return -1;
    }
    result=val1+val2;
    printf("%d + %d = %d\n", val1, val2, result);

    // Subtract the two values
    if (ioctl(fd, MYDEMODRV_SUB, &result) < 0) {
        printf("Failed to perform sub operation\n");
        close(fd);
        return -1;
    }
    result=val1-val2;
    printf("%d - %d = %d\n", val1, val2, result);

    // Find the maximum of the two values
    if (ioctl(fd, MYDEMODRV_MAX, &result) < 0) {
        printf("Failed to perform max operation\n");
        close(fd);
        return -1;
    }
    result=val1>val2?val1:val2;
    printf("max(%d, %d) = %d\n", val1, val2, result);

    // Read the result from the device
    if (read(fd, &result, sizeof(int)) < 0) {
        printf("Failed to read result from device\n");
        close(fd);
        return -1;
    }
    //printf("Result read from device: %d\n", result);

    // Close the device
    close(fd);

    return 0;
}

