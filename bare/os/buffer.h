//
//
//
//
//



#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>


struct Buffer
{
  void *buffer; //pointer to the start of the memory area. static or dynamic
        
  uint32_t buffer_size; // size of buffer. Needs to be 2^N - 1.

  uint32_t write; //starts at 0
  uint32_t read; //starts at 0
                    
}; //12 bytes

static inline uint32_t BufferIsEmpty(struct Buffer *buffer)
{
	return buffer->write == buffer->read ? 1 : 0;
}

uint32_t BufferPut8(struct Buffer *buffer, uint8_t *data);

uint32_t BufferGet8(struct Buffer *buffer, uint8_t *data);

uint32_t BufferWrite8(struct Buffer *buffer, uint8_t *data, uint32_t num_data);
uint32_t BufferRead8(struct Buffer *buffer, uint8_t *data, uint32_t num_data);

uint32_t BufferPut16(struct Buffer *buffer, uint16_t *data);

uint32_t BufferGet16(struct Buffer *buffer, uint16_t *data);

uint32_t BufferPut32(struct Buffer *buffer, uint32_t *data);

uint32_t BufferGet32(struct Buffer *buffer, uint32_t *data);

#endif
