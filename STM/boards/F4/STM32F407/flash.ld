/*
*****************************************************************************
**
**  File        : stm32_flash.ld
**
**  Abstract    : Linker script for STM32F407VG Device with
**                1024KByte FLASH, 192KByte RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Environment : Atollic TrueSTUDIO(R)
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) toolchain.
**
*****************************************************************************
*/



/* Entry Point */
ENTRY(Reset_Handler)



/* Highest address of the user mode stack */
_estack = 0x20020000;    /* end of 128K RAM on AHB bus*/



/* Specify the memory areas */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K /* Flash */
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K /* SRAM */
  CCM (rwx)				: ORIGIN = 0x60000000, LENGTH = 64K /* Core Coupled Memory*/
}



/* Define output sections */
SECTIONS
{


	/* The Interrupt vector table is always first */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Vector Table */
    . = ALIGN(4);
  } >FLASH



  /* The program code */
  .text :
  {
    . = ALIGN(4);
    *(.text)           
    *(.text*)          
    . = ALIGN(4);

  } >FLASH



	/* Constant data */
	.rodata :
	{
		. = ALIGN(4);

    *(.rodata)         
    *(.rodata*)        

		. = ALIGN(4);
	} >FLASH



		/* ARM ELF UNWINDING SECTION
   .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
    .ARM : {
    __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
    } >FLASH
		*/



	/* uninitialized data */
  .bss (NOLOAD) :
  {
		. = ALIGN(4);
    _sbss = .;         /* bss start in ram*/

    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* bss end in ram*/
  } >RAM

	


  /* initialized data */

	/* data start in flash */
  _sidata = LOADADDR(.data);
  
	.data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* data start in ram */

    *(.data)           
    *(.data*)         

    . = ALIGN(4);
    _edata = .;        /* data end in ram */
  } >RAM AT> FLASH



  	/* Core Coupled Memory RAM */

	/* data start in flash */
  _siccm = LOADADDR(.ccm);
  
	.ccm :
  {
		. = ALIGN(4);
    _sccm = .;         /* ccm start*/
   
	 *(.ccm)
	 *(.ccm*)
	 *(.CCM)
	 *(.CCM*)

		. = ALIGN(4);
    _eccm = .;         /* ccm end*/
	} >CCM AT> FLASH



  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

	/*I dont know what this does */
  .ARM.attributes 0 : { *(.ARM.attributes) }
}
