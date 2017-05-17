//
//
//
//
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

struct CommunicationConfig
{
	void *data_in; //pointer to the data to communicate through
	void *data_out;
	uint32_t num_data; //number of data to communicate
	uint32_t timeout_milli; //timeout in milliseconds for the communication
};











#endif
