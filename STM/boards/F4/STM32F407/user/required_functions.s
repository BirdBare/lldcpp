





 .syntax unified
  .thumb

.section  .text.switch






	.global DEVICE_CREATE_REGISTER_STACK
	.type DEVICE_CREATE_REGISTER_STACK, %function
DEVICE_CREATE_REGISTER_STACK:
	push {r4-r10} //save registers because we have to
	mov r9, #0x1000000 //set PSR Reset Value
	mov r8, r1 //move functions address to program counter
	ldr r7, =BAREOS_THREAD_RETURN //link register is return func
	mov r6, #0 //set r12 to zero. usually zero for new functions
	stmdb r0!, {r2-r9} //push essential registers to stack
  mov r10, #0xFFFFFFFD //exception return with psp thread mode
	stmdb r0!, {r1-r10} //push dont care registers and exception
	pop {r4-r10} //put saved registers back
bx lr

	










.global BARE_SCHEDULER_SWITCH_PROLOGUE
	.type BARE_SCHEDULER_SWITCH_PROLOGUE, %function
BARE_SCHEDULER_SWITCH_PROLOGUE:
	ldr r2, =BAREOS_SCHEDULER//get scheduler base address
	ldr r1, [r2, #0]//get current thread
	ldr r0, [r2, #4]//get next thread at list
bx lr












.global BARE_SCHEDULER_SWITCH_EPILOGUE
	.type BARE_SCHEDULER_SWITCH_EPILOGUE, %function
BARE_SCHEDULER_SWITCH_EPILOGUE:
	ldr r1, [r0]//load next thread
	str r1, [r2, #4]//store next thread in list.
	str r0, [r2, #0]//store loaded thread as current thread
bx lr















	.global PendSV_Handler
	.type PendSV_Handler, %function
PendSV_Handler:
	
	push {r12,lr}

	bl BARE_SCHEDULER_SWITCH_PROLOGUE //REQUIRED STARTING FUNCTION
	
//ACTUAL SWITCH 

	mrs r12, psp//get psp address
	stmdb r12!, {r3-r11, lr}//store rest of registers for switch
	str r12, [r1, #12]//store stack pointer in current thread

	ldr r12, [r0, #12]//load new thread stack pointer
	ldmia r12!, {r3-r11, lr}//load new thread registers
	msr psp, r12//store new stack pointer for thread in psp

//END SWITCH

	bl BARE_SCHEDULER_SWITCH_EPILOGUE //REQUIRE ENDING CALL FUNCTION

	pop {r12,lr}

bx lr

