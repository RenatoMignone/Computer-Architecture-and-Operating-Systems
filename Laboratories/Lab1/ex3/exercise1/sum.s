// Write a program that sums the first 30 numbers and store them into register r1
// Run and test the program using https://cpulator.01xz.net/?sys=arm

.word stack_top     // Address of the stack_top
.word _start        // Address of the _start function
.thumb_func 

.global _start
_start:
	mov r0, #0 			// Initialize sum to 0       
	mov r1, #1        	// Initialize counter to 1
loop:
	add r0, r0, r1    	// Add counter to sum
	add r1, r1, #1      // Increment counter
	cmp r1, #31         // Compare counter with 31
	blt loop      		// If counter < 31, branch to loop    
end:
    b .

