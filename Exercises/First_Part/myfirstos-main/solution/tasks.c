extern void my_printf(const char *s); 


// Task 1 implementation
void task1(void) {
    my_printf("Task 1 is running...\n");
    for (volatile int i = 0; i < 5; i++) {
        // Simulating task work
    }
    my_printf("Task 1 is done.\n");
}

// Task 2 implementation
void task2(void) {
    my_printf("Task 2 is running...\n");
    for (volatile int i = 0; i < 5; i++) {
        // Simulating task work
    }
    my_printf("Task 2 is done.\n");
}


