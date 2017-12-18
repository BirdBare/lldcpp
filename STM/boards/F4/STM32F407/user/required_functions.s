





 .syntax unified
  .thumb

.section  .text.switch

	
	.global DEVICE_CREATE_REGISTER_STACK
	.type DEVICE_CREATE_REGISTER_STACK, %function
DEVICE_CREATE_REGISTER_STACK:
	
	push {r4-r10} //save registers because we have to
	mov r9, #0x1000000 //set PSR Reset Value
	mov r8, r1 //move functions address to program counter
	ldr r7, =BARE_THREAD_RETURN //link register is return func
	mov r6, #0 //set r12 to zero. usually zero for new functions
	stmdb r0!, {r2-r9} //push essential registers to stack
  mov r10, #0xFFFFFFFD //exception return with psp thread mode
	stmdb r0!, {r1-r10} //push dont care registers and exception
	pop {r4-r10} //put saved registers back
bx lr

