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

  if(next != buffer->read)
  {
    *(uint8_t *)(buffer->buffer + buffer->write) = *data;
                            
    buffer->write = next;
                                        
    return 1;
  }
  return 0;
}

uint32_t BufferGet8(struct Buffer *buffer, uint8_t *data)
{
  uint32_t readpos = buffer->read;

  if(readpos != buffer->write)
  {
    *data = *(uint8_t *)(buffer->buffer + readpos);
                        
    readpos++;
                                    
    buffer->read = readpos & buffer->buffer_size; 
                                                    
    return 1;
  }

  return 0;
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



// Buffer 16

uint32_t BufferPut16(struct Buffer *buffer, uint16_t *data)
{
  uint32_t next = (buffer->write + 1) % buffer->buffer_size;

  if(next != buffer->read)
  {
    *(uint16_t *)(buffer->buffer + buffer->write) = *data;
                            
    buffer->write = next;
                                        
    return 1;
  }
  return 0;
}

uint32_t BufferGet16(struct Buffer *buffer, uint16_t *data)
{
  uint32_t readpos = buffer->read;

  if(readpos != buffer->write)
  {
    *data = *(uint16_t *)(buffer->buffer + readpos);
                        
    readpos++;
                                    
    buffer->read = readpos % buffer->buffer_size; 
                                                    
    return 1;
  }

  return 0;
}


//Buffer 32

uint32_t BufferPut32(struct Buffer *buffer, uint32_t *data)
{
  uint32_t next = (buffer->write + 1) % buffer->buffer_size;

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
  uint32_t readpos = buffer->read;

  if(readpos != buffer->write)
  {
    *data = *(uint32_t *)(buffer->buffer + readpos);
                        
    readpos++;
                                    
    buffer->read = readpos % buffer->buffer_size; 
                                                    
    return 1;
  }

  return 0;
}


