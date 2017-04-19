//
//
//
//
//




#include "SYSCFG_lld.h"

struct SYSCFGxo
	SYSCFGo = {{0x44,14},0,SYSCFG};

void SYSCFG_Config(struct SYSCFGxo *SYSCFGo, int MEMRMP, int CBR, int CMPCR)
{
	SYSCFG->MEMRMP = MEMRMP;
	SYSCFG->CBR = CBR;
	SYSCFG->CMPCR = CMPCR;
}


void SYSCFG_ConfigEXTI(struct SYSCFGxo *SYSCFGo, int EXTIHigh, int EXTIMidHigh, 
	int EXTIMidLow, int EXTILow)
{
	SYSCFG->EXTICR[3] = EXTIHigh;	
	SYSCFG->EXTICR[2] = EXTIMidHigh;	
	SYSCFG->EXTICR[1] = EXTIMidLow;	
	SYSCFG->EXTICR[0] = EXTILow;	
}




