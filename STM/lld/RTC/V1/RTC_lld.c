//
//
//
//
//



#include "RTC_lld.h"


struct RTCxo
	RTCo = {{0x40,10},0,RTC};


void RTC_Unlock(struct RTCxo *RTCo)
{
	volatile RTC_TypeDef * const RTCx = RTCo->RTCx;
	
	RTCx->WPR = 0xCA;
	RTCx->WPR = 0x53;
}


void RTC_Lock(struct RTCxo *RTCo)
{
	RTCo->RTCx->WPR = 0xff;
}


int RTC_ConfigEnable(struct RTCxo *RTCo, int TR, int DR, int CR, int PRER, 
	int TAMPCR, int OR)
{
	volatile RTC_TypeDef * const RTCx = RTCo->RTCx;

	if((RTCx->ISR & RTC_ISR_INIT) == 0)
	{
		RTCx->ISR |= RTC_ISR_INIT;

		while((RTCx->ISR & RTC_ISR_INITF) == 0)
		{
			asm volatile("");
		}

		RTCx->PRER = PRER;
		RTCx->CR = (RTCx->CR & 0b00000000111110000111011100000111) | CR;
		RTCx->DR = DR;
		RTCx->TR = TR;

		return 0;
	}
	return RTC_CONFIG_INIT;
}


int RTC_ConfigAlarmA(struct RTCxo *RTCo, int CR, int ALRMAR, int ALRMASSR)
{
	volatile RTC_TypeDef * const RTCx = RTCo->RTCx;

	if((RTCx->CR & RTC_CR_ALRAE) == 0)
	{
		RTCx->ALRMAR = ALRMAR;
		RTCx->ALRMASSR = ALRMASSR;
		RTCx->CR = (RTCx->CR & ~RTC_CR_ALRAIE) | CR;

		return 0;
	}
	return RTC_CONFIG_AENABLED;

}


int RTC_ConfigAlarmB(struct RTCxo *RTCo, int CR, int ALRMBR, int ALRMBSSR)
{
	volatile RTC_TypeDef * const RTCx = RTCo->RTCx;

	if((RTCx->CR & RTC_CR_ALRBE) == 0)
	{
		RTCx->ALRMBR = ALRMBR;
		RTCx->ALRMBSSR = ALRMBSSR;
		RTCx->CR = (RTCx->CR & ~RTC_CR_ALRBIE) | CR;

		return 0;
	}
	return RTC_CONFIG_BENABLED;
}



int RTC_ConfigPeriodicAlarm(struct RTCxo *RTCo, int CR, int WUTR)
{
	volatile RTC_TypeDef * const RTCx = RTCo->RTCx;

	if((RTCx->CR & RTC_CR_WUTE) == 0)
	{
		RTCx->WUTR = WUTR;
		RTCx->CR = (RTCx->CR & ~(RTC_CR_WUTIE | RTC_CR_WUCKSEL)) | CR;

		return 0;
	}
	return RTC_CONFIG_WUTENABLED;
}


int RTC_ConfigCalibration(struct RTCxo *RTCo, int CR, int CALR);



int RTC_ResetConfig(struct RTCxo *RTCo)
{
	volatile RTC_TypeDef * const RTCx = RTCo->RTCx;

	if((RTCx->CR & (RTC_CR_WUTE | RTC_CR_ALRBE | RTC_CR_ALRAE)) == 0)
	{
		RTCx->CR = 0;
		RTCx->TR = 0;
		RTCx->DR = 0;

		return 0;
	}
	return RTC_CONFIG_ENABLED;
}


void RTC_EnableAlarmA(struct RTCxo *RTCo)
{
	RTCo->RTCx->CR |= RTC_CR_ALRAE;
}



void RTC_EnableAlarmB(struct RTCxo *RTCo)
{
	RTCo->RTCx->CR |= RTC_CR_ALRBE;
}



void RTC_EnablePeriodicAlarm(struct RTCxo *RTCo)
{
	RTCo->RTCx->CR |= RTC_CR_WUTE;
}



int RTC_EnableCalibration(struct RTCxo *RTCo);




void RTC_Get32(struct RTCxo *RTCo, int DataArray[3])
{
	volatile RTC_TypeDef * const RTCx = RTCo->RTCx;

	DataArray[0] = RTCx->DR;
	DataArray[0] = RTCx->TR;
	DataArray[0] = RTCx->SSR;
}













