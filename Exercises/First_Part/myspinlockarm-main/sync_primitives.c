// sync_primitives.c
#include "sync_primitives.h"

void spinlock_init(spinlock_t *lock) {
  //0 if the spin is free, 1
  lock->lock = 0;
}
void spinlock_acquire(spinlock_t *lock) {
  //test if the spinlock is free, if yes set to 1, then busy wait
  int result ;
  do{
    //read curent value of the spinlock, if 0 we set it to 1
    //we will use some asm instructions that only work on armv7
          __asm__ volatile (
                //we need to set te output of this to a var
                //the letter r in this case is because we want the compiler
                //to put the result into a register, the & we do not want that register to be used
                //for anything else, = means that this is a write only value.
                "ldrex %0, [%1]" //go in memo, use the address in %1 that is the first input parameter here, and store it in %0 that is the output. (the register)
                //ex is for exclusive, form the moment you load the value in memo, a special block in the microprocessor that continuously check that memory address, when we change it there will be a flag. We wil now be able to ask the microprocessor if someone has tried to change the value in that given memory address.
                "cmp %0,#0\n" //comparing the register with the spinlock with 0
                "bne retry_label\n" //lock is already acquire
                "strex %0,%2,[%1]\n "//if we are here the spinlock is free, we do not need a normal store, but strex, we sotre %0 so result into the memory address held by %1, we are trying to save the value of the spinlock. %2 is a register where we get a result about if the store failed or not. we try to store %2 into the memo address of %1then we store the result of the store in %0.
                "cmp %0, #0\n" //here we are comparing the rsult of the strex with 0, if it is equal to 0, it means that the store failed, if not we can exit. 
                "bne retry_label\n"
                "b exit_label\n"
                "retry_label:\n"
                "nop\n" //completely useless
                "exit_label:\n"
                
                //the important parts are the the ex parts, ldrex and strex, if there is a change in between, the store will fail or the load, and redo it again. This is because we want our operations to be atomic.
                //this solution works both with single core or multiple core.
                
                :"=&r" (result)
                //the first part (&lock) is the address of the lock
                :"r" (&lock->lock), "r" (1) //this notation, we do not want
                :"cc","memory" //cc means that whatever i am writing changes the value in the registers, memory means that my code is going to access memo but i do not know which address, so i tell the compiler to do not make any assumpions for the address.
                

    );
    
  } while(result!=0);
}


void spinlock_release(spinlock_t *lock) {
  __asm__ volatile (
        "str %1, [%0]\n"            // Set lock state to 0 (unlocked)
        :
        : "r" (&lock->lock), "r" (0)
        : "memory"
    );

}
