#include <stdint.h>

#define STACK_SIZE 4096

uint32_t stack_task1[STACK_SIZE];
uint32_t stack_task2[STACK_SIZE];

uint32_t *sp_task1 = &stack_task1[STACK_SIZE-1];
uint32_t *sp_task2 = &stack_task2[STACK_SIZE-1];

extern void my_printf(const char *s);

extern void task1(void);
extern void task2(void);

void switch_task(uint32_t **current_sp, uint32_t **next_sp){
	
	__asm volatile(
		"mov %0,sp\n"

		: 
		: "=r" (*current_sp)
			
	);

	__asm volatile (
		"mov sp, %0\n"
		:
		: "r" (*next_sp)

	);
}



int main(void) {

	uint32_t *current_sp;



	while(1){
		
		switch_task(&current_sp, &sp_task1);
		task1();

		switch_task(&current_sp, &sp_task1);
		task2();
	}

    return 0;

}


