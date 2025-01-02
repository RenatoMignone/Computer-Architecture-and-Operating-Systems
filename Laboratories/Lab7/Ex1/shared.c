#include "shared.h"

void array_to_string(int *array, size_t size, char *str){

	size_t offset = 0;
	int bytes_written;

	for(int i=0; i < size; i++){

		/* Write integer into string with sprintf, available also in the kernel */
		bytes_written = sprintf(str + offset, "%d", array[i]);

		/* Move pointer ahead of number of written bytes */
		offset += bytes_written;

        if (i < size - 1) {

            /* Add a comma after every number except the last one */
            *(str + offset) = ',';
        }

		else{

            /* Null terminate the string */
            *(str + offset) = '\0';
		}

		/* Move pointer ahead of one position to attach the next element*/
		offset += 1;

	}

}

void string_to_array(const char *input, int *output, int max_size) {

    int i = 0;
	int num = 0;
	int count = 0;
    int is_negative = 0;
	int is_number = 1;

    while (input[i] != '\0' && count < max_size) {

        /* Skip whitespace and commas */
        if (input[i] == ' ' || input[i] == ',') {
            i++;
			is_number = 0;
        }

		if(is_number){

			/* Handle negative numbers */
			if (input[i] == '-') {
				is_negative = 1;
				i++;
			}

			/* Convert the digits to integer */
			while (input[i] >= '0' && input[i] <= '9') {

				/* Convert from ASCII */
				num = num * 10 + (input[i] - '0');
				i++;
			}

			/* Apply negative sign if necessary */
			if (is_negative) {
				num = -num;
				is_negative = 0;
			}

			/* Store the number in the array */
			output[count++] = num;

			/* Reset the number for the next token */
			num = 0;
		}

		is_number = 1;

    }

}
