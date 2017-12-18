//
//
//
//
//


#include "bareos.h"

void BAREOS_THREAD_RETURN(void)
{
BREAK(50);
}











struct BareOSThread * BareOSThreadCreateThread(void *thread_memory, 
	void (*thread_function)(void *args), void *args, uint32_t stack_size)
{
	struct BareOSThread *new_thread = thread_memory;
	//we are creating the new bare therad on the memory area

	new_thread->next = new_thread;
	new_thread->prev = new_thread;
	//must point to itself at first

	new_thread->flags = 0;
	//flag is set to zero and configured by user

	stack_size -= sizeof(struct BareOSThread);

	new_thread->stack_size = stack_size;
	new_thread->stack_pointer = thread_memory + stack_size;
	//set size and pointer start

	void * DEVICE_CREATE_REGISTER_STACK(void *e_stack, 
		void (*thread_function)(void *args),	void *args);		
	new_thread->stack_pointer = DEVICE_CREATE_REGISTER_STACK(
		new_thread->stack_pointer, thread_function, args);
	//call user created function that stacks thread registers for context switch
	//set new stack_pointer location with function return value

	return new_thread;
}











