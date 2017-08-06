//
//
//
//
//


  .syntax unified


  .section  .text.Switch
  .type  ContextSwitch, %function
ContextSwitch:
//r0 = thread switched out
//r1 = thread switched in

ldr r0, [SCHEDULER]

mrs r2, psp //get current thread stack pointer memory address

mrs r3, CONTROL //get control register 

stmdb r0!, {r4-r11} //save callee save registers





bx lr


