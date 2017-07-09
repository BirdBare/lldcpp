//
//
//
//
//

#ifndef DEBUG_H
#define DEBUG_H

#ifndef DEBUG_USART
#error "MUST DEFINE DEBUG USART"
#endif

#include "usart_lld.h"

#define DebugMessage(message) \
do \
{ \
	struct CommunicationConfig DMCC = {message,0,sizeof(message),0}; \
	UsartWrite8Buffer(&DEBUG_USART, &DMCC); \
} while(0);








#endif
