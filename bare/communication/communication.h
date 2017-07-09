//
//
//
//
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

struct CommunicationConfig
{
	void *tx_data; 
	void *rx_data;
	uint32_t num_data; //number of data to communicate
	uint32_t timeout_milli; //timeout in milliseconds for the communication
};











#endif
