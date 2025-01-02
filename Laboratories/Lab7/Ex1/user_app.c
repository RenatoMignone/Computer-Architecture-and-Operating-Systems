#include <stdio.h>

/* Include open */ 
#include <fcntl.h>

/* Include close */ 
#include <unistd.h>

/* Include exit */ 
#include <stdlib.h>

#include "shared.h"

int main(void) 
{ 
	int file_desc; 
	int bytes_read;
	int array[5] = {1, 2, 367, 44, 5};
	int array_size;

	char write_buffer[BUF_LEN];
	char read_buffer[BUF_LEN];

	array_size = sizeof(array) / sizeof(int);

	if(array_size > MAX_INT_COUNT){
		printf("Provided array is too big\n");
		return -1;
	}
 
    file_desc = open(DEVICE_PATH, O_RDWR); 

    if (file_desc < 0) { 

		printf("Can't open device file: %s, error:%d\n", DEVICE_PATH,
				file_desc); 

        exit(EXIT_FAILURE); 
    } 

	array_to_string(array, array_size, write_buffer);

	printf("Your array is: %s\n", write_buffer);

	write(file_desc, write_buffer, BUF_LEN);

	read(file_desc, read_buffer, BUF_LEN);

	printf("The sum is: %s\n", read_buffer);

    close(file_desc); 

    return 0; 
}
