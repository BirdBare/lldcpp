//
//
//
//
//



#ifndef STM32F767_DMA2D_H
#define STM32F767_DMA2D_H

#include "board.h"
#include "FMC_LCD.h"
#include "PERIPH.h"
#include "RCC_lld.h"

struct DMA2Dxo
{
	struct RCCxo RCCo;

	uint16_t unused2;

	volatile DMA2D_TypeDef * const DMA2Dx;
};


extern struct DMA2Dxo
	DMA2Do;

#define DMA2D_CONFIG_ENABLED 1

int DMA2D_Config(struct DMA2Dxo *DMA2Do, int CR, int LWR, int AMTCR);

int DMA2D_ResetConfig(struct DMA2Dxo *DMA2Do);

int DMA2D_EnableRegToMem(struct DMA2Dxo *DMA2Do, int OPFCCR, int OCOLR, 
	void *OMAR, int OOR, int NLR);


int DMA2D_EnableMemToMem(struct DMA2Dxo *DMA2Do, void *FGMAR, int FGOR,
	int FGPFCCR, void *OMAR, int OOR, int NLR);

int DMA2D_EnableMemToMemPFC(struct DMA2Dxo *DMA2Do, void *FGMAR, int FGOR,
	int FGPCFCR, int OPCFCR, void *OMAR, int OOR, int NLR);


void DMA2D_Disable(struct DMA2Dxo *DMA2Do);










#define LCD_COLORMODE DMA2D_COLORMODE_RGB565

#define DMA2D_COLORMODE_RGB565 0b010

#define LCD_XYOOR(x, y) ((x) > 239 || (y) > 319)


void DMA2D_CopyPixelMap(const uint16_t map[], uint32_t x, uint32_t y, uint32_t
w, uint32_t h);

#define DMA2D_WaitTransfer(void) \
  PERIPH_WaitTillReset(&DMA2D->CR,DMA2D_CR_START)

#define RGB565 0b010
#define RGB332 0b101






#define DMA2D_LoadCLUT(void) \
do \
{ \
  DMA2D->FGCMAR = (uint32_t)&DMA2D_CLUT[0]; \
  DMA2D->BGCMAR = (uint32_t)&DMA2D_CLUT[0]; \
  DMA2D_WaitTransfer(); \
  DMA2D->FGPFCCR = DMA2D_FGPFCCR_START | (255 << 8); \
  PERIPH_WaitTillReset(&DMA2D->FGPFCCR, DMA2D_FGPFCCR_START); \
  DMA2D->BGPFCCR = DMA2D_BGPFCCR_START | (255 << 8); \
  PERIPH_WaitTillReset(&DMA2D->BGPFCCR, DMA2D_BGPFCCR_START); \
} while(0)



void DMA2D_CopyPixelMapPFC(const uint8_t map[], uint16_t x, uint16_t y, uint16_t
w, uint16_t h);

/*
OPFCCR Colorformat
OCOLR  Color to write
OMAR   Write Address
NLR    NUMBER LINES and pixels
OOR    LINE OFFSET
*/

#define DMA2D_Reg2Mem(OMAR, OPFCCR, OCOLR, NLR, OOR) \
do{ \
  DMA2D_WaitTransfer(); \
	DMA2D_EnableRegToMem(&DMA2Do, OPFCCR, OCOLR, (void *)(OMAR), OOR, NLR); \
} while(0)


/*
FGPFCCR Colorformat
OCOLR  Color to write
OMAR   Write Address
FGMAR  Input address
NLR    NUMBER LINES and pixels
OOR    LINE OFFSET
FGOR    LINE OFFSET
*/


#define DMA2D_Mem2Mem(FGMAR, OMAR, FGPFCCR, NLR, FGOR, OOR) \
do{ \
  DMA2D_WaitTransfer(); \
  DMA2D_EnableMemToMem(&DMA2Do, (void *)(FGMAR), FGOR, FGPFCCR, (void*)(OMAR), \
		OOR, NLR); \
} while(0)


/*
FGPFCCR input Colorformat
OPFCCR  ouput color format
OCOLR  Color to write
OMAR   Write Address
FGMAR  Input address
NLR    NUMBER LINES and pixels
OOR    LINE OFFSET
FGOR    LINE OFFSET
*/

#define DMA2D_Mem2MemPFC(FGMAR, OMAR, FGPFCCR, OPFCCR, NLR, FGOR, OOR) \
do{ \
  DMA2D_WaitTransfer(); \
	DMA2D_EnableMemToMemPFC(&DMA2Do, (void *)(FGMAR), FGOR, FGPFCCR, OPFCCR, \
		(void *)(OMAR), OOR, NLR); \
} while(0)


















// 8 BIT TO 32 BIT COLOR LOOKUP TABLE
extern const uint32_t DMA2D_CLUT [256];


#endif





