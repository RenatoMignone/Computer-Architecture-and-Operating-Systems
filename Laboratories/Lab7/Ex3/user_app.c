#include <stdio.h>

/* Include open */ 
#include <fcntl.h>

/* Include close */ 
#include <unistd.h>

/* Include exit */ 
#include <stdlib.h>

/* Include ioctl */
#include <sys/ioctl.h> /* ioctl */ 

#include "shared.h"

int main(void) 
{ 
    int file_desc; 
	int bytes_read;
	char write_buffer[BUF_LEN] = "Hello world!\n";
	char read_buffer[BUF_LEN];

    file_desc = open(DEVICE_PATH, O_RDWR); 

    if (file_desc < 0) { 

		printf("Can't open device file: %s, error:%d\n", DEVICE_PATH,
				file_desc); 

        exit(EXIT_FAILURE); 
    } 

	write(file_desc, write_buffer, BUF_LEN);

	read(file_desc, read_buffer, BUF_LEN);

	ioctl(file_desc, IOCTL_SET_BAUDRATE);
	ioctl(file_desc, IOCTL_SET_PARITY);
	ioctl(file_desc, IOCTL_SET_STOPBITS);

    close(file_desc); 

    return 0; 
}
