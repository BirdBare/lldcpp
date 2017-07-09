//
//
//
//
//



#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>


struct Buffer8
{
  uint8_t *buffer; //pointer to the start of the memory area. static or dynamic
        
  uint32_t buffer_size; // size of buffer. Needs to be 2^N - 1.

  uint32_t write; //starts at 0
  uint32_t read; //starts at 0
                    
};

uint32_t Buffer8Put(struct Buffer8 *buffer, uint8_t *data);

uint32_t Buffer8Get(struct Buffer8 *buffer, uint8_t *data);

uint32_t Buffer8Write(struct Buffer8 *buffer, uint8_t *data, uint32_t num_data);
uint32_t Buffer8Read(struct Buffer8 *buffer, uint8_t *data, uint32_t num_data);

struct Buffer16
{
  uint16_t *buffer;
        
  uint32_t write;
  uint32_t read;
                    
  uint32_t buffer_size;
};

uint32_t Buffer16Put(struct Buffer16 *buffer, uint16_t *data);

uint32_t Buffer16Get(struct Buffer16 *buffer, uint16_t *data);

struct Buffer32
{
  uint32_t *buffer;
        
  uint32_t write;
  uint32_t read;
                    
  uint32_t buffer_size;
};

uint32_t Buffer32Put(struct Buffer32 *buffer, uint32_t *data);

uint32_t Buffer32Get(struct Buffer32 *buffer, uint32_t *data);

#endif
