//
//
//
//
//



#ifndef STM32F767_CACHE_H
#define STM32F767_CACHE_H

#include "board.h"
//#include "STM32F767_.h"

#define CACHE_SETS 127
#define CACHE_WAYS 3

#define CACHE_SET_OFFSET 5
#define CACHE_WAY_OFFSET 30

void CACHE_InvalidateICache(void);

void CACHE_InvalidateICacheByAddress(void *address, uint32_t size);

void CACHE_EnableICache(void);

void CACHE_DisableICache(void);



void CACHE_InvalidateDCache(void);

void CACHE_CleanDCache(void);

void CACHE_CleanInvalidateDCache(void);

void CACHE_InvalidateDCacheByAddress(void *address, uint32_t size);

void CACHE_CleanDCacheByAddress(void *address, uint32_t size);




void CACHE_CleanInvalidateDCacheByAddress(void *address, uint32_t size);

void CACHE_EnableDCache(void);



void CACHE_DisableDCache(void);





#endif
