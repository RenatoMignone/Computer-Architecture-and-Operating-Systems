.global _start
_start:
    mov r0, #10          // N = 10, calculate the 10th Fibonacci number
    bl fibonacci         // Call the Fibonacci function
    // Result will be in r0

end:
    b end                // Infinite loop to halt the program


fibonacci:
    cmp r0, #1           // Check if n <= 1
    ble base_case        // If yes, branch to base_case

    // Recursive case: Fib(n) = Fib(n-1) + Fib(n-2)
    push {r0, r1, lr}    // Save registers r0, r1, and link register

    sub r0, r0, #1       // Calculate Fib(n-1)
    bl fibonacci         // Recursive call
    mov r1, r0           // Store result of Fib(n-1) in r1

    pop {r0}             // Restore original n value
    sub r0, r0, #2       // Calculate Fib(n-2)
    bl fibonacci         // Recursive call

    add r0, r0, r1       // Sum Fib(n-1) + Fib(n-2) for Fib(n)

    pop {r1, lr}         // Restore r1 and the link register
    bx lr                // Return


base_case:
    // Base cases: if n = 0, return 0; if n = 1, return 1
    mov r0, r0           // Leave r0 as is if n = 0 or set r0 to 1 if n = 1
    bx lr                // Return
