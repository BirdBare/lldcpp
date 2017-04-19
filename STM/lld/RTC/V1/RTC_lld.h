//
//
//
//
//



#ifndef STM32F767_RTC_H
#define STM32F767_RTC_H

#include "board.h"
#include "RCC_lld.h"

struct RTCxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile RTC_TypeDef * const RTCx;
};

extern struct RTCxo
	RTCo;

void RTC_Unlock(struct RTCxo *RTCo);
void RTC_Lock(struct RTCxo *RTCo);

#define RTC_CONFIG_INIT 1
#define RTC_CONFIG_ENABLED 1
#define RTC_CONFIG_AENABLED 1
#define RTC_CONFIG_BENABLED 1
#define RTC_CONFIG_WUTENABLED 1
int RTC_ConfigEnable(struct RTCxo *RTCo, int TR, int DR, int CR, int PRER, 
	int TAMPCR, int OR);

int RTC_ConfigAlarmA(struct RTCxo *RTCo, int CR, int ALRMAR, int ALRMASSR);

int RTC_ConfigAlarmB(struct RTCxo *RTCo, int CR, int ALRMBR, int ALRMABSR);

int RTC_ConfigPeriodicAlarm(struct RTCxo *RTCo, int CR, int WUTR);

int RTC_ConfigCalibration(struct RTCxo *RTCo, int CR, int CALR);

int RTC_ResetConfig(struct RTCxo *RTCo);

void RTC_EnableAlarmA(struct RTCxo *RTCo);

void RTC_EnableAlarmB(struct RTCxo *RTCo);

void RTC_EnablePeriodicAlarm(struct RTCxo *RTCo);

int RTC_EnableCalibration(struct RTCxo *RTCo);

void RTC_Get32(struct RTCxo *RTCo, int DataArray[3]);

#endif
