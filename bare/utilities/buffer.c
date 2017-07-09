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

uint32_t Buffer8Put(struct Buffer8 *buffer, uint8_t *data)
{
  uint32_t next = (buffer->write + 1) & buffer->buffer_size;

  if(next != buffer->read)
  {
    *(buffer->buffer + buffer->write) = *data;
                            
    buffer->write = next;
                                        
    return 1;
  }
  return 0;
}

uint32_t Buffer8Get(struct Buffer8 *buffer, uint8_t *data)
{
  uint32_t readpos = buffer->read;

  if(readpos != buffer->write)
  {
    *data = *(buffer->buffer + readpos);
                        
    readpos++;
                                    
    buffer->read = readpos & buffer->buffer_size; 
                                                    
    return 1;
  }

  return 0;
}

uint32_t Buffer8Write(struct Buffer8 *buffer, uint8_t *data, uint32_t num_data)
{
	for(uint32_t count = 0; count < num_data; count++)
	{
		uint32_t next = (buffer->write + 1) & buffer->buffer_size;

		if(next == buffer->read)
		{
			return count;
		}

		*(buffer->buffer + buffer->write) = data[count];
                            
		buffer->write = next;
	}

  return num_data;

}


uint32_t Buffer8Read(struct Buffer8 *buffer, uint8_t *data, uint32_t num_data)
{
	for(uint32_t count = 0; count < num_data; count++)
	{
		uint32_t readpos = buffer->read;

		if(readpos == buffer->write)
		{
			return count;
		}
			*data++ = *(buffer->buffer + readpos);
                        
			readpos++;
                                    
			buffer->read = readpos & buffer->buffer_size; 
	}
	return num_data;
}



// Buffer 16

uint32_t Buffer16Put(struct Buffer16 *buffer, uint16_t *data)
{
  uint32_t next = (buffer->write + 1) % buffer->buffer_size;

  if(next != buffer->read)
  {
    *(buffer->buffer + buffer->write) = *data;
                            
    buffer->write = next;
                                        
    return 1;
  }
  return 0;
}

uint32_t Buffer16Get(struct Buffer16 *buffer, uint16_t *data)
{
  uint32_t readpos = buffer->read;

  if(readpos != buffer->write)
  {
    *data = *(buffer->buffer + readpos);
                        
    readpos++;
                                    
    buffer->read = readpos % buffer->buffer_size; 
                                                    
    return 1;
  }

  return 0;
}


//Buffer 32

uint32_t Buffer32Put(struct Buffer32 *buffer, uint32_t *data)
{
  uint32_t next = (buffer->write + 1) % buffer->buffer_size;

  if(next != buffer->read)
  {
    *(buffer->buffer + buffer->write) = *data;
                            
    buffer->write = next;
                                        
    return 1;
  }
  return 0;
}

uint32_t Buffer32Get(struct Buffer32 *buffer, uint32_t *data)
{
  uint32_t readpos = buffer->read;

  if(readpos != buffer->write)
  {
    *data = *(buffer->buffer + readpos);
                        
    readpos++;
                                    
    buffer->read = readpos % buffer->buffer_size; 
                                                    
    return 1;
  }

  return 0;
}


