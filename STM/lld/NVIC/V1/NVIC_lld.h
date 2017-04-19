//
//
//
//
//



#ifndef STM32F767_NVIC_H
#define STM32F767_NVIC_H

#include "board.h"

void NVIC_Config(IRQn_Type IRQn, int PRIO);
void NVIC_ConfigSystem(IRQn_Type IRQn, int PRIO);
uint32_t NVIC_GetConfig(IRQn_Type IRQn);
uint32_t NVIC_GetConfigSystem(IRQn_Type IRQn);

void NVIC_Enable(IRQn_Type IRQn);

void NVIC_Disable(IRQn_Type IRQn);





#endif
