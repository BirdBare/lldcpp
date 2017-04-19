//
//
//
//
//




#include "FPU_lld.h"

void FPU_Config(int FPCCR, int FPCAR, int FPDSCR)
{
	FPU->FPCCR = FPCCR;
	FPU->FPCAR = FPCAR;

}







