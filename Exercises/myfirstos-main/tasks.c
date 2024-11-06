external void my_printf(const char *s);


void task1(void){
	my_printf("Task 1 is running...\n");
	for(volatile int i=0; i<5; i++){
		
	}
	
	my_printf("Task 1 is done.\n");
	
}


void task2(void){
	my_printf("Task 2 is running...\n");
        for(volatile int i=0; i<5; i++){
        
        }

        my_printf("Task 2 is done.\n");
	
}
