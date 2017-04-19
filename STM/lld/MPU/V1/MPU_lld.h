//
//
//
//
//



#ifndef STM32F767_MPU_H
#define STM32F767_MPU_H

#include "board.h"
#include "BARE_DEFINES.h"

//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void MPU_Enable(int CTRL) 
{
	MPU->CTRL = CTRL;
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void MPU_Disable(void) 
{
	MPU->CTRL = 0;
}

/* RegionNum IS FROM 0-7 */

/* MemoryAddressStart is the RegionSize Aligned memory address start */
//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void MPU_SetRegion(int RegionNum, int MemoryAddressStart, 
	int InstructionFetch, int AccessPermissions, int MemoryAccessAttributes, 
	int RegionSize) 
{
  MPU->RNR = RegionNum; 
	
  MPU->RBAR = MemoryAddressStart; 
	
  MPU->RASR = ((InstructionFetch) << 28) | ((AccessPermissions) << 24) | ((MemoryAccessAttributes) << 16) | ((RegionSize << 1)); 
}

//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void MPU_EnableRegion(int RegionNum) 
{
  MPU->RNR = RegionNum; 
	
  MPU->RASR |= 1; 
} 

//******************************************************************************
//	
//										 
//	
//******************************************************************************
ALWAYS_INLINE void MPU_DisableRegion(int RegionNum) 
{
  MPU->RNR = RegionNum; 
  
	MPU->RASR &= ~1;
} 





#endif
