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

	struct Buffer8 buffer = {buffer_memory, 255};

	uint8_t hello[] = "HELLO";

	//TEST PUT AND GET

	for(int count = 0; count < 5; count++)
	{
		Buffer8Put(&buffer, &hello[count]);
	}

	for(int count = 0; count < 5; count++)
	{
		uint8_t data; 
		Buffer8Get(&buffer, &data);
		printf("%c", data);
	}
	//FINISH PUT AND GET

	//TEST WRITE AND READ
	for(int count = 0; count < 5; count++)
	{
		Buffer8Write(&buffer, hello,6);
	}

	for(int count = 0; count < 5; count++)
	{
		char data[6]; 
		Buffer8Read(&buffer, &data, 6);
			printf("%s", data);

	}
	//FINISH WRITE AND READ

	//OVERLOAD TEST
	for(int count2 = 0; count2 < 25; count2++)
	{
		for(int count1 = 0; count1 < 5; count1++)
		{
			printf("P%i",Buffer8Put(&buffer, &hello[count1]));
		}
			printf("W%i",Buffer8Write(&buffer, hello,6));
	}

for(int count2 = 0; count2 < 25; count2++)
	{
		for(int count1 = 0; count1 < 5; count1++)
		{
			printf("P%i",Buffer8Get(&buffer, &hello[count1]));
		}
			printf("W%i",Buffer8Read(&buffer, hello,6));
	}
	//FINISH OVERLOAD



	



return 0;
}
