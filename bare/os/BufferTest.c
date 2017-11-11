// This is a testing mechanism to test the functions of the buffer for correct
// operation
//
//
//

#include "buffer.h"
#include <stdio.h>


int main(void)
{
	uint8_t buffer_memory[256];

	struct Buffer buffer = {buffer_memory, 255};

	uint8_t hello[] = "HELLO";

	//TEST PUT AND GET

	for(int count = 0; count < 5; count++)
	{
		BufferPut8(&buffer, &hello[count]);
	}

	for(int count = 0; count < 5; count++)
	{
		uint8_t data; 
		BufferGet8(&buffer, &data);
		printf("%c", data);
	}
	//FINISH PUT AND GET

	//TEST WRITE AND READ
	for(int count = 0; count < 5; count++)
	{
		BufferWrite8(&buffer, hello,6);
	}

	for(int count = 0; count < 5; count++)
	{
		char data[6]; 
		BufferRead8(&buffer, &data, 6);
			printf("%s", data);

	}
	//FINISH WRITE AND READ

	//OVERLOAD TEST
	for(int count2 = 0; count2 < 25; count2++)
	{
		for(int count1 = 0; count1 < 5; count1++)
		{
			printf("P%i",BufferPut8(&buffer, &hello[count1]));
		}
			printf("W%i",BufferWrite8(&buffer, hello,6));
	}

for(int count2 = 0; count2 < 25; count2++)
	{
		for(int count1 = 0; count1 < 5; count1++)
		{
			printf("P%i",BufferGet8(&buffer, &hello[count1]));
		}
			printf("W%i",BufferRead8(&buffer, hello,6));
	}
	//FINISH OVERLOAD



	



return 0;
}
