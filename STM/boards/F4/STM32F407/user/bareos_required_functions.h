//
//
//
//
//

#ifndef BAREOS_REQUIRED_FUNCTIONS_H
#define BAREOS_REQUIRED_FUNCTIONS_H


//System Init


//Interrupt handler
void PendSV_Handler(void)
{
	BareOSSchedulerSwitch();
}


//call interruot handler
void BareOSCallSwitch(void)
{
	SCB->ICSR |= 1 << 28;
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
}

//user supplied register switching function
static inline void DEVICE_SAVE_REGISTERS_STACK(void)
{
//save current thread
asm volatile("mrs r1, psp"); //get stack pointer address

//Possibly stack fpu registers if in use
asm volatile("mov r2, #0b1 << 4"); //for getting fpu context
asm volatile("ands r2, lr"); //get context flag value for it block
asm volatile("it eq"); //if then block
asm volatile("vstmdbeq r1!, {s16-s31}"); //if bit is reset then fpu is used
//end fpu stack

//save current thread
asm volatile("stmdb r1!, {r3-r11, lr}"); //store rest of thread registers
asm volatile("str r1, [r0, #0]"); //store stack pointer address in thread struct
}

static inline void DEVICE_LOAD_REGISTERS_STACK(void)
{
//load next thread
asm volatile("ldr r1, [r0]"); //get stack pointer address
asm volatile("ldmia r1!, {r3-r11, lr}"); //load most of thread registers

//Possibly stack fpu registers if in use
asm volatile("mov r2, #0b1 << 4"); //for getting fpu context
asm volatile("ands r2, lr"); //get context flag value for it block
asm volatile("it eq"); //if then block
asm volatile("vldmiaeq r1!, {s16-s31}"); //if bit is reset then fpu is used
//end fpu stack
	
	//load next thread
	asm volatile("msr psp, r1"); //store stack pointer
	}


__attribute__((noinline))
void * DEVICE_CREATE_REGISTERS_STACK(void * e_stack,
 void(*thread_function)(void *args), void *args)
{
asm volatile("push {r4-r10}"); //save registers because we have to
asm volatile("mov r9, #0x1000000"); //set PSR Reset Value
asm volatile("mov r8, r1"); //move functions address to program counter
asm volatile("ldr r7, =BAREOS_THREAD_RETURN"); //link register is return func
asm volatile("mov r6, #0"); //set r12 to zero. usually zero for new functions
asm volatile("stmdb r0!, {r2-r9}"); //push essential registers to stack
asm volatile("mov r10, #0xFFFFFFFD"); //exception return with psp	thread mode
asm volatile("stmdb r0!, {r1-r10}"); //push dont care registers and exception
asm volatile("pop {r4-r10}"); //put saved registers back
return e_stack;
}





























#endif
