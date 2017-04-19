//
//
//
//
//




#include "FMC_lld.h"
#include "RCC_lld.h"




// ######     IMPORTANT      ###############
//
//			NOR_PSRAM1 must be disabled before it can be used. Reset Value has
//			MBKEN set. No Others are affected.

struct NOR_PSRAMxo
	NOR_PSRAM1o = {{0x38,0},0,NOR_PSRAM1},
	NOR_PSRAM2o = {{0x38,0},0,NOR_PSRAM2},
	NOR_PSRAM3o = {{0x38,0},0,NOR_PSRAM3},
	NOR_PSRAM4o = {{0x38,0},0,NOR_PSRAM4};


int NOR_PSRAM_Config(struct NOR_PSRAMxo *NOR_PSRAMo, int BCR, int BTR, int BWTR)
{
	volatile NOR_PSRAM_TypeDef * const NOR_PSRAMx = NOR_PSRAMo->NOR_PSRAMx;

	if((NOR_PSRAMx->BCR & FMC_BCR1_MBKEN) == 0)
	{
		if((BCR & FMC_BCR1_WFDIS) != 0)
		{
			NOR_PSRAM1->BCR |= FMC_BCR1_WFDIS;
		}
		//check write fifo disable bit

		if((BCR & FMC_BCR1_CCLKEN) != 0)
		{
			NOR_PSRAM1->BCR |= FMC_BCR1_CCLKEN;
		}
		//check continuous clock enable bit

		NOR_PSRAMx->BCR = BCR;
		NOR_PSRAMx->BTR = BTR;
		NOR_PSRAMx->BWTR = BWTR;


		return 0;
	}
	return NOR_PSRAM_CONFIG_ENABLED;
}

int NOR_PSRAM_ResetConfig(struct NOR_PSRAMxo *NOR_PSRAMo)
{	
	volatile NOR_PSRAM_TypeDef * const NOR_PSRAMx = NOR_PSRAMo->NOR_PSRAMx;

	if((NOR_PSRAMx->BCR & FMC_BCR1_MBKEN) == 0)
	{
		NOR_PSRAMx->BCR = 0;
		NOR_PSRAMx->BTR = 0;
		NOR_PSRAMx->BWTR = 0;

		return 0;
	}
	return NOR_PSRAM_CONFIG_ENABLED;
}

void NOR_PSRAM_Enable(struct NOR_PSRAMxo *NOR_PSRAMo)
{
	NOR_PSRAMo->NOR_PSRAMx->BCR |= FMC_BCR1_MBKEN;
}

void NOR_PSRAM_Disable(struct NOR_PSRAMxo *NOR_PSRAMo)
{
	NOR_PSRAMo->NOR_PSRAMx->BCR &= ~FMC_BCR1_MBKEN;
}





















