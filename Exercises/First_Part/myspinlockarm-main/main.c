#include <stdio.h>
#include "sync_primitives.h"
#include <stdint.h>
#include <stdio.h>

extern void spinlock_init(spinlock_t *spinlock);
extern void spinlock_acquire(spinlock_t *spinlock);
extern void spinlock_release(spinlock_t *spinlock);
extern void my_printf(const char *s); 
extern void intToStr(int num, char *str); 


void critical_section() {
    // Example critical section code
    my_printf("In critical section.\n");
}


int main() {
    char message[20];
    spinlock_t my_lock;
    // Initialize the spinlock to unlocked
    my_printf("Starting spinlock initialization \n");
    spinlock_init(&my_lock);
    intToStr(my_lock.lock,message);
    my_printf("Spinlock initialized to ");
    my_printf(message);
    my_printf("\n");

    // Acquire the spinlock
    my_printf("Starting acquire spinlock \n");

    spinlock_acquire(&my_lock);
    
    intToStr(my_lock.lock,message);
    my_printf("Spinlock value is ");
    my_printf(message);
    my_printf("\n");

    // Enter critical section
    critical_section();

    // Release the spinlock
    my_printf("Starting release spinlock \n");
    spinlock_release(&my_lock);
    intToStr(my_lock.lock,message);
    my_printf("Spinlock value is ");
    my_printf(message);
    my_printf("\n");

    return 0;
}

