#ifndef WORKER_H
#define WORKER_H

#include <vector>

void do_compare(unsigned char* LSB, int LSB_length, int nr_bits, 
		unsigned char* start_search, unsigned char* end_search, int search_length);

#endif
