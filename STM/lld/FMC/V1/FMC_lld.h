//
//
//
//
//



#ifndef STM32F767_FMC_H
#define STM32F767_FMC_H

#include "board.h"

#define NOR_PSRAM1 \
	((NOR_PSRAM_TypeDef *)((uint32_t)FMC_Bank1_R_BASE + 0x0))
#define NOR_PSRAM2 \
	((NOR_PSRAM_TypeDef *)((uint32_t)FMC_Bank1_R_BASE + 0x8))
#define NOR_PSRAM3 \
	((NOR_PSRAM_TypeDef *)((uint32_t)FMC_Bank1_R_BASE + 0x10))
#define NOR_PSRAM4 \
	((NOR_PSRAM_TypeDef *)((uint32_t)FMC_Bank1_R_BASE + 0x18))

typedef struct
{
	__IO uint32_t BCR; //0x0

	__IO uint32_t BTR; //0x4

			 uint32_t reserved[63];//0x8

	__IO uint32_t BWTR; //0x104
}	NOR_PSRAM_TypeDef;




#include "RCC_lld.h"

struct NOR_PSRAMxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile NOR_PSRAM_TypeDef * const NOR_PSRAMx;
};

extern struct NOR_PSRAMxo
	NOR_PSRAM1o,
	NOR_PSRAM2o,
	NOR_PSRAM3o,
	NOR_PSRAM4o;


  


/*
  Parameter value ranges:
    AddSetup 0 - 15 (15 default) = number HCLK cycles
    AddHold 1 - 15 (15 default) = number HCLK cycles 
    DataSetup 1 - 255 (255 default) = number HCLK cycles 
    DataLatency 2 - 17 (17 default) = number CLK cycles 
    BusTurn 0 - 15 (15 default) = number HCLK cycles 
    ClkDiv 2 - 16 (16 default) = number HCLK cycles 


*/

#define FMC_WriteBurst_ON FMC_BCR1_CBURSTRW 
#define FMC_WriteBurst_OFF 0x0

#define FMC_PageSize_0 0x0
#define FMC_PageSize_128 (uint32_t)0x10000
#define FMC_PageSize_256 (uint32_t)0x20000
#define FMC_PageSize_512 (uint32_t)0x30000
#define FMC_PageSize_1024 (uint32_t)0x40000

#define FMC_AsyncWait_ON FMC_BCR1_ASYNCWAIT 
#define FMC_AsyncWait_OFF 0x0

#define FMC_ExtendedMode_ON FMC_BCR1_EXTMOD 
#define FMC_ExtendedMode_OFF 0x0

#define FMC_SyncWait_ON FMC_BCR1_WAITEN
#define FMC_SyncWait_OFF 0x0

#define FMC_WriteEnable_ON FMC_BCR1_WREN
#define FMC_WriteEnable_OFF 0x0

#define FMC_WaitConfig_BEFORE 0x0
#define FMC_WaitConfig_DURING FMC_BCR1_WAITCFG 

#define FMC_WrapMode_ON FMC_BCR1_WRAPMOD
#define FMC_WrapMode_OFF 0x0

#define FMC_WaitPolarity_LOW 0x0
#define FMC_WaitPolarity_HIGH FMC_BCR1_WAITPOL  

#define FMC_BurstEnable_ON FMC_BCR1_BURSTEN 
#define FMC_BurstEnable_OFF 0x0

#define FMC_FlashAccess_ON FMC_BCR1_FACCEN 
#define FMC_FlashAccess_OFF 0x0

#define FMC_BusWidth_8 0x0
#define FMC_BusWidth_16 FMC_BCR1_MWID_0 
#define FMC_BusWidth_32 FMC_BCR1_MWID_1 

#define FMC_MemType_SRAM 0x0
#define FMC_MemType_PSRAM FMC_BCR2_MTYP_0 
#define FMC_MemType_NOR FMC_BCR2_MTYP_1

#define FMC_MultiplexEnable_OFF 0x0
#define FMC_MultiplexEnable_ON

#define FMC_AccessMode_A 0x0 
#define FMC_AccessMode_B FMC_BTR1_ACCMOD_0
#define FMC_AccessMode_C FMC_BTR1_ACCMOD_1  
#define FMC_AccessMode_D (FMC_BTR1_ACCMOD_0 | FMC_BTR1_ACCMOD_1)  





#define NOR_PSRAM_CONFIG_ENABLED 1
int NOR_PSRAM_Config(struct NOR_PSRAMxo *NOR_PSRAMo, int BCR, int BTR, int BWTR);
int NOR_PSRAM_ResetConfig(struct NOR_PSRAMxo *NOR_PSRAMo);
void NOR_PSRAM_Enable(struct NOR_PSRAMxo *NOR_PSRAMo);
void NOR_PSRAM_Disable(struct NOR_PSRAMxo *NOR_PSRAMo);
























#endif



