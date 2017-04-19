//
//
//
//
//



#include "CACHE_lld.h"

void CACHE_InvalidateICache(void)
{
	__DSB();
	__ISB();

	SCB->ICIALLU = 0;

	__DSB();
	__ISB();
}


void CACHE_InvalidateICacheByAddress(void *address, uint32_t size)
{
	__DSB();
	__ISB();

	do
	{
		SCB->ICIMVAU = (uint32_t)address;
		address = (void *)((uint32_t)address + 32);
		size -= 32;
	} while(size > 0);


	__DSB();
	__ISB();
}

void CACHE_EnableICache(void)
{
	CACHE_InvalidateICache();

	SCB->CCR |= SCB_CCR_IC_Msk;

	__DSB();
	__ISB();
}


void CACHE_DisableICache(void)
{
	__DSB();
	__ISB();

	SCB->CCR &= ~SCB_CCR_IC_Msk;

	__DSB();
	__ISB();
}



void CACHE_InvalidateDCache(void)
{
	__DSB();
	__ISB();

	uint32_t sets = CACHE_SETS;

	do
	{
		uint32_t ways = CACHE_WAYS;

		do
		{
			SCB->DCISW = (ways << CACHE_WAY_OFFSET) | (sets << CACHE_SET_OFFSET);

		} while(ways--);

	} while(sets--);

	__DSB();
	__ISB();
}


void CACHE_CleanDCache(void)
{
	__DSB();
	__ISB();

	uint32_t sets = CACHE_SETS;

	do
	{
		uint32_t ways = CACHE_WAYS;

		do
		{
			SCB->DCCSW = (ways << CACHE_WAY_OFFSET) | (sets << CACHE_SET_OFFSET);

		} while(ways--);

	} while(sets--);

	__DSB();
	__ISB();
}

void CACHE_CleanInvalidateDCache(void)
{
	__DSB();
	__ISB();

	uint32_t sets = CACHE_SETS;

	do
	{
		uint32_t ways = CACHE_WAYS;

		do
		{
			SCB->DCCISW = (ways << CACHE_WAY_OFFSET) | (sets << CACHE_SET_OFFSET);

		} while(ways--);

	} while(sets--);

	__DSB();
	__ISB();
}

void CACHE_InvalidateDCacheByAddress(void *address, uint32_t size)
{
	__DSB();
	__ISB();

	do
	{
		SCB->DCIMVAC = (uint32_t)address;
		address = (void *)((uint32_t)address + 32);
		size -= 32;
	} while(size > 0);


	__DSB();
	__ISB();
}


void CACHE_CleanDCacheByAddress(void *address, uint32_t size)
{
	__DSB();
	__ISB();

	do
	{
		SCB->DCCMVAC = (uint32_t)address;
		address = (void *)((uint32_t)address + 32);
		size -= 32;
	} while(size > 0);


	__DSB();
	__ISB();
}




void CACHE_CleanInvalidateDCacheByAddress(void *address, uint32_t size)
{
	__DSB();
	__ISB();

	do
	{
		SCB->DCCIMVAC = (uint32_t)address;
		address = (void *)((uint32_t)address + 32);
		size -= 32;
	} while(size > 0);


	__DSB();
	__ISB();
}

void CACHE_EnableDCache(void)
{
	CACHE_InvalidateDCache();

	SCB->CCR |= SCB_CCR_DC_Msk;

	__DSB();
	__ISB();
}



void CACHE_DisableDCache(void)
{
	CACHE_CleanDCache();

	SCB->CCR &= ~SCB_CCR_DC_Msk;

	__DSB();
	__ISB();
}





