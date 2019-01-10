//
//
//
//
//

#include "main.hpp"


void Tick(uint32_t tick)
{
	for(uint32_t i=0; i<tick; i++)
	 asm volatile("");
}

//******************************************************************************
//
//						GPIO LED BLINK FUNCTIONS
//
//******************************************************************************
void EnableLed(void)
{
	RCC->APB2ENR |= 0b1 << 3;
	//enables gpio clock

	GPIOB->CRH &=	~(0b1111 << 4);
	GPIOB->CRH |= 0b0001 << 4;
	//enables gpio led pin functionality on pin 9 GPIOB
}
//finished

void ToggleLed(void)
{
	GPIOB->BSRR |= 0b1 << 9;
}
//finished
//******************************************************************************
//
//			END
//
//******************************************************************************




//******************************************************************************
//		
//		Spi Functions
//
//******************************************************************************
void EnableSpi(void)
{
	RCC->APB2ENR |= 0b1 << 3;
	//enables gpio B clock
	
	GPIOB->CRL &= ~(0b1111 << 12); 
	GPIOB->CRL |= 0b1011 << 12;
	//sets pin 3 as alt push pull for spi clock

	GPIOB->CRL &= ~(0b1111 << 20); 
	GPIOB->CRL |= 0b1011 << 20;
	//sets pin 5 as alt push pull for spi data

	RCC->APB2ENR |= 0b1 << 0;
	//enables AFIO clock

	AFIO->MAPR &= ~(0b111 << 24);
	AFIO->MAPR |= 0b010 << 24;

	AFIO->MAPR &= ~(0b1 << 0);
	AFIO->MAPR |= (0b1 << 0);
	//remaps spi pins to chosen pins pb3 and pb5.

	RCC->APB2ENR |= 0b1 << 12;
	//enables spi clock
	
	SPI1->CR2 |= SPI_CR2_SSOE;
	//must be set to work

	SPI1->CR1 &= 0;
	SPI1->CR1 |= (!SPI_CR1_DFF) | !SPI_CR1_LSBFIRST | SPI_CR1_SPE |
	SPI_CR1_BR_1 | SPI_CR1_MSTR | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
	//enable master, clock/128, enabled, lsb first, 8 bit format
}
//finished

void WaitSpi(void)
{
	while((SPI1->SR & SPI_SR_TXE) == 0)
	{
		Tick(1);
	}
	//waits for the TXE flag to be set. meaning we can send more data.
}

void SendDataSpi(uint8_t data)
{	
	
	ToggleLed();

	WaitSpi();

	SPI1->DR = data;
	//send the data
	
	
}
//finished

//******************************************************************************
//
//		end
//
//******************************************************************************




//******************************************************************************
//
//			LCD Functions
//
//******************************************************************************
void EnableLcd(void)
{
	EnableSpi();

	RCC->APB2ENR |= 0b1 << 3;
	//enables gpio B clock

	GPIOB->CRL &= ~(0b1111 << 16);
	GPIOB->CRL |= 0b0011 << 16;
	//sets pin 4 as gpio output for setting reset on lcd

	GPIOB->CRH &= ~(0b1111 << 0);
	GPIOB->CRH |= 0b0011 << 0;
	//sets pin 8 for cmd/data selectin for lcd

	GPIOB->BSRR = 0b1 << 4;
	//set reset pin because pin is active low

}

void ResetLcd(void)
{
	
	GPIOB->BSRR = 0b1 << 20;
	Tick(5000);
	GPIOB->BSRR = 0b1 << 4;
	Tick(5000);
}

void CommandLcd(uint8_t command)
{
		SendDataSpi(command);

	GPIOB->BSRR = 0b1 << 24;
	//reset cmd/data pin

}

void DataLcd(uint8_t data)
{
		SendDataSpi(data);

	GPIOB->BSRR = 0b1 << 8;
	//set cmd/data pin

}

void InitLcd(void)
{
	ResetLcd();


	CommandLcd(0x1);
	//software reset
	Tick(5000);

	//ILI9163 LCD
	CommandLcd(0x11);
	Tick(5000);
	//sleep out

	CommandLcd(0x3a);
	DataLcd(0x5);
	//set color mode

	CommandLcd(0x36);
	DataLcd(0b00100000);
	//111 correct. 001 upside down
	//set mem order

	CommandLcd(0x26);
	DataLcd(0x4);
	//gamma curve

	CommandLcd(0xf2);
	DataLcd(0x1);
	//gamma adjustment enabled

	CommandLcd(0xE0);
	DataLcd(0x3f);
	DataLcd(0x25);
	DataLcd(0x1c);
	DataLcd(0x1e);
	DataLcd(0x20);
	DataLcd(0x12);
	DataLcd(0x2a);
	DataLcd(0x90);
	DataLcd(0x24);
	DataLcd(0x11);
	DataLcd(0x00);
	DataLcd(0x00);
	DataLcd(0x00);
	DataLcd(0x00);
	DataLcd(0x00);
	CommandLcd(0xE1);
	DataLcd(0x20);
	DataLcd(0x20);
	DataLcd(0x20);
	DataLcd(0x20);
	DataLcd(0x5);
	DataLcd(0x0);
	DataLcd(0x15);
	DataLcd(0xa7);
	DataLcd(0x3d);
	DataLcd(0x18);
	DataLcd(0x25);
	DataLcd(0x2a);
	DataLcd(0x2b);
	DataLcd(0x2b);
	DataLcd(0x3a);
	//gamma adjustment	

	CommandLcd(0xb1);
	DataLcd(0x8);
	DataLcd(0x8);
	//frame rate control
	
	CommandLcd(0xb4);
	DataLcd(0x7);
	//display inversion control

	CommandLcd(0xc0);
	DataLcd(0xa);
	DataLcd(0x2);
	// power control 1

	CommandLcd(0xc1);
	DataLcd(0x2);
	//power control 2

	CommandLcd(0xc5);
	DataLcd(0x50);
	DataLcd(0x5b);
	//power control 3

	CommandLcd(0xc7);
	DataLcd(0x40);
	//power control 4

	CommandLcd(0x13);
	//normal display 

	CommandLcd(0x29);
	//display on

	Tick(5000);


}

void SetAreaLcd(uint8_t HStart, uint8_t HEnd, uint8_t VStart, uint8_t VEnd)
{
	CommandLcd(0x2a);
	DataLcd(0);
	DataLcd(HStart);
	DataLcd(0);
	DataLcd(HEnd);



	CommandLcd(0x2b);
	DataLcd(0);
	DataLcd(VStart);
	DataLcd(0);
	DataLcd(VEnd);
}

void WritePixelLcd(uint8_t x, uint8_t y, uint16_t color)
{
	SetAreaLcd(x,x,y,y);
	CommandLcd(0x2c);
	DataLcd(color >> 8);
	DataLcd(color);


}

//******************************************************************************
//
//		end LCD
//
//******************************************************************************

void EnableI2c(void)
{
	RCC->APB1ENR |= 0b1 << 21;

}




int main(void)
{
	//volatile SPI_TypeDef *SPI = SPI1;
	//volatile AFIO_TypeDef *AFI = AFIO;
	//volatile GPIO_TypeDef *B = GPIOB;


	EnableLed();
	EnableLcd();
	InitLcd();
	





	while(1)
	{
for(int k=0; k<130; k++)
for(int i=0+k; i<130+k; i++)
		for(int j=0+k; j<130+k; j++)
		{
			WritePixelLcd(i-k,j-k,0xff*k);
		}
	

	}	

	return 0;
}

