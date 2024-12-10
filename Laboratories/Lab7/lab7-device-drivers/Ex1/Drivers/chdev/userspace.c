#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
    const char *device = "/dev/chardev";
    int fd;
    int data_to_write[] = {1, 2, 3, 4, 5}; // Data to write
    int sum = 0; // Variable to store the result from read
    ssize_t ret;

    // Open the device
    fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return EXIT_FAILURE;
    }
    printf("Device opened successfully.\n");

    // Write data to the device
    ret = write(fd, data_to_write, sizeof(data_to_write));
    if (ret < 0) {
        perror("Failed to write to the device");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Data written to the device successfully.\n");

    // Read data (the sum) from the device
    ret = read(fd, &sum, sizeof(sum));
    if (ret < 0) {
        perror("Failed to read from the device");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Sum of array elements received from device: %d\n", sum);

    // Close the device
    if (close(fd) < 0) {
        perror("Failed to close the device");
        return EXIT_FAILURE;
    }
    printf("Device closed successfully.\n");

    return EXIT_SUCCESS;
}
