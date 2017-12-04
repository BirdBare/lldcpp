//
//
//
//
//



#include "buffer.h"



//******************************************************************************
//	
//									Buffer 8 functions 	 
//	
//******************************************************************************
uint32_t BufferPut8(struct Buffer *buffer, uint8_t *data)
{
  uint32_t next = (buffer->write + 1) & buffer->buffer_size;
	//find place to move write pointer

  if(next != buffer->read)
  {
    *(uint8_t *)(buffer->buffer + buffer->write) = *data;
		//write the data in current spot                           

    buffer->write = next;
		//move buffer write to next place

    return 0;
  }
	//only run if the next place is not where buffer read is.
  return 1;
}

uint32_t BufferGet8(struct Buffer *buffer, uint8_t *data)
{
  uint32_t readpos = buffer->read;
	//get read pointer

  if(readpos != buffer->write)
  {
    *data = *(uint8_t *)(buffer->buffer + readpos);
		//read the data
                        
    readpos++;
		//move pointer
                                    
    buffer->read = readpos & buffer->buffer_size; 
    //get next place to read data

    return 0;
  }
	//read pointer must always be one behind the write pointer.

  return 1;
}

uint32_t BufferWrite8(struct Buffer *buffer, uint8_t *data, uint32_t num_data)
{
	for(uint32_t count = 0; count < num_data; count++)
	{
		uint32_t next = (buffer->write + 1) & buffer->buffer_size;

		if(next == buffer->read)
		{
			return count;
		}

		*(uint8_t *)(buffer->buffer + buffer->write) = data[count];
                            
		buffer->write = next;
	}
	//loop for num_data

  return num_data;

}


uint32_t BufferRead8(struct Buffer *buffer, uint8_t *data, uint32_t num_data)
{
	for(uint32_t count = 0; count < num_data; count++)
	{
		uint32_t readpos = buffer->read;

		if(readpos == buffer->write)
		{
			return count;
		}
			*data++ = *(uint8_t *)(buffer->buffer + readpos);
                        
			readpos++;
                                    
			buffer->read = readpos & buffer->buffer_size; 
	}
	return num_data;
}



//******************************************************************************
//
//															Buffer 16
//
//******************************************************************************

// Buffer 16

uint32_t BufferPut16(struct Buffer *buffer, uint16_t *data)
{
	if((buffer->write & 0b1) != 0 || (buffer->read & 0b1) != 0)
	{
		return 1;
	}
	//make sure both buffers are on a number divisible by two.
	
  uint32_t next = (buffer->write + 2) & buffer->buffer_size;

  if(next != buffer->read)
  {
    *(uint16_t *)(buffer->buffer + buffer->write) = *data;
                            
    buffer->write = next;
                                        
    return 0;
  }
  return 1;
}

uint32_t BufferGet16(struct Buffer *buffer, uint16_t *data)
{
	if((buffer->write & 0b1) != 0 || (buffer->read & 0b1) != 0)
	{
		return 1;
	}
	//make sure both buffers are on a number divisible by two.
	
  uint32_t readpos = buffer->read;

  if(readpos != buffer->write)
  {
    *data = *(uint16_t *)(buffer->buffer + readpos);
                        
    readpos+=2;
                                    
    buffer->read = readpos & buffer->buffer_size; 
                                                    
    return 0;
  }

  return 1;
}

uint32_t BufferWrite16(struct Buffer *buffer, uint16_t *data, uint32_t num_data)
{
	if((buffer->write & 0b1) != 0 || (buffer->read & 0b1) != 0)
	{
		return 1;
	}
	//make sure both buffers are on a number divisible by two.

	for(uint32_t count = 0; count < num_data; count++)
	{
		uint32_t next = (buffer->write + 2) & buffer->buffer_size;

		if(next == buffer->read)
		{
			return count;
		}

		*(uint8_t *)(buffer->buffer + buffer->write) = data[count];
                            
		buffer->write = next;
	}

  return num_data;

}


uint32_t BufferRead16(struct Buffer *buffer, uint16_t *data, uint32_t num_data)
{
	if((buffer->write & 0b1) != 0 || (buffer->read & 0b1) != 0)
	{
		return 1;
	}
	//make sure both buffers are on a number divisible by two.

	for(uint32_t count = 0; count < num_data; count++)
	{
		uint32_t readpos = buffer->read;

		if(readpos == buffer->write)
		{
			return count;
		}
			*data++ = *(uint8_t *)(buffer->buffer + readpos);
                        
			readpos+=2;
                                    
			buffer->read = readpos & buffer->buffer_size; 
	}
	return num_data;
}




//Buffer 32

uint32_t BufferPut32(struct Buffer *buffer, uint32_t *data)
{
	if((buffer->write & 0b11) != 0 || (buffer->read & 0b11) != 0)
	{
		return 1;
	}
	//make sure both buffers are on a number divisible by 4.

  uint32_t next = (buffer->write + 4) & buffer->buffer_size;

  if(next != buffer->read)
  {
    *(uint32_t *)(buffer->buffer + buffer->write) = *data;
                            
    buffer->write = next;
                                        
    return 1;
  }
  return 0;
}

uint32_t BufferGet32(struct Buffer *buffer, uint32_t *data)
{
	if((buffer->write & 0b11) != 0 || (buffer->read & 0b11) != 0)
	{
		return 1;
	}
	//make sure both buffers are on a number divisible by 4.

  uint32_t readpos = buffer->read;

  if(readpos != buffer->write)
  {
    *data = *(uint32_t *)(buffer->buffer + readpos);
                        
    readpos+=4;
                                    
    buffer->read = readpos & buffer->buffer_size; 
                                                    
    return 1;
  }
  return 0;
}

uint32_t BufferWrite32(struct Buffer *buffer, uint32_t *data, uint32_t num_data)
{
	if((buffer->write & 0b11) != 0 || (buffer->read & 0b11) != 0)
	{
		return 1;
	}
	//make sure both buffers are on a number divisible by 4.

	for(uint32_t count = 0; count < num_data; count++)
	{
		uint32_t next = (buffer->write + 4) & buffer->buffer_size;

		if(next == buffer->read)
		{
			return count;
		}

		*(uint8_t *)(buffer->buffer + buffer->write) = data[count];
                            
		buffer->write = next;
	}
  return num_data;
}


uint32_t BufferRead32(struct Buffer *buffer, uint32_t *data, uint32_t num_data)
{
	if((buffer->write & 0b11) != 0 || (buffer->read & 0b11) != 0)
	{
		return 1;
	}
	//make sure both buffers are on a number divisible by 4.

	for(uint32_t count = 0; count < num_data; count++)
	{
		uint32_t readpos = buffer->read;

		if(readpos == buffer->write)
		{
			return count;
		}
			*data++ = *(uint8_t *)(buffer->buffer + readpos);
                        
			readpos+=4;
                                    
			buffer->read = readpos & buffer->buffer_size; 
	}
	return num_data;
}


