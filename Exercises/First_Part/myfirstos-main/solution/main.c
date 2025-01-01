#include <stdint.h>
  
#define STACK_SIZE 4096 
// Define two stacks for each task
uint32_t stack_task1[STACK_SIZE];
uint32_t stack_task2[STACK_SIZE];

// Declare the stack pointers for each task
uint32_t *sp_task1 = &stack_task1[STACK_SIZE - 1];
uint32_t *sp_task2 = &stack_task2[STACK_SIZE - 1];

extern void my_printf(const char *s); 

  
// Function prototypes
extern void task1(void);
extern void task2(void);

// Function to switch tasks
void switch_task(uint32_t **current_sp, uint32_t **next_sp) {
    __asm volatile (
        "mov %0, sp\n"       // Save current stack pointer to *current_sp
        : "=r" (*current_sp) 
    );

    __asm volatile (
        "mov sp, %0\n"       // Set the stack pointer to *next_sp
        :
        : "r" (*next_sp)
    );
}

int main(void) {

        uint32_t *current_sp;
	    // Infinite loop to simulate the round-robin task switcher
    while (1) {
        // Switch to task 1
        switch_task(&current_sp, &sp_task1);
        task1();  // Run task1 (this will loop forever in this basic example)

        // Switch to task 2
        switch_task(&current_sp, &sp_task2);
        task2();  // Run task2 (this will loop forever in this basic example)
    }

    return 0;
}


