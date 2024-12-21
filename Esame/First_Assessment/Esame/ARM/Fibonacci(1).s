.section .text
.global _start

_start:
    MOV r0, #7
	MOV r1, #0
	MOV r2,#0
	MOV r3,#1
    BL fibonacci
    B exit

fibonacci:
    CMP r0, #1
    BLE end_fib

    SUB r0, r0, #1
    PUSH {r0, lr}
    BL fibonacci
    POP {r1, lr}
    ADD r2, r3, r0 @f(x) = f(x-1) + f(x-2)
	MOV r0,r3
	MOV r3,r2
	BX lr

end_fib:
	MOV r0,#0
    BX lr

exit:
    B exit
